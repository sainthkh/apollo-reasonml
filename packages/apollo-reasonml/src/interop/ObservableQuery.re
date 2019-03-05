open GraphqlTypes;
open QueryTypes;

module type QueryConfig = {
  let client: ApolloClient.apolloClient;

  type schemaQueryResponse;
  let decodeResponse: Js.Json.t => schemaQueryResponse;
  type variablesType;
  let encodeVariables: variablesType => Js.Json.t;

  type errorExtension;
  let decodeExtension: Js.Json.t => errorExtension;
};

module Make = (Q: QueryConfig) => {
  let queryObservable: Pervasives.ref(
    option(observableQuery)
  ) = ref(None);

  let subscription: Pervasives.ref(
    option(subscription)
  ) = ref(None);

  let init = (
    ~query: gqlQuery,
    ~variables: option(Q.variablesType),
    ~fetchPolicy: fetchPolicy,
    ~errorPolicy: errorPolicy,
    ~pollInterval: int,
    ~notifyOnNetworkStatusChange: bool,
    ()
  ) => {
    let opts = observableQueryOpts(
      ~variables = switch(variables) {
      | None => None
      | Some(v) => Some(Q.encodeVariables(v))
      },
      ~fetchPolicy = string_of_fetchPolicy(fetchPolicy),
      ~errorPolicy = string_of_errorPolicy(errorPolicy),
      ~query=query,
      ~pollInterval=pollInterval,
      ~notifyOnNetworkStatusChange=notifyOnNetworkStatusChange
    );

    queryObservable := Some(Q.client##watchQuery(opts));
  };

  type graphqlError = {
    message: string, 
    locations: option(array(sourceLocation)),
    path: option(array(string)),
    source: option(gqlSource),
    positions: option(array(int)),
    extensions: Q.errorExtension,
  };

  type apolloError = {
    message: string,
    graphqlErrors: array(graphqlError),
    networkError: option(Js.Exn.t),
  };

  let errorToRecord = (error:apolloErrorJs) => {
    message: error##message,
    graphqlErrors: decodeGraphqlErrors(error##graphqlErrors, Q.decodeExtension),
    networkError: Js.Nullable.toOption(error##networkError),
  }

  type apolloCurrentQueryResult = {
    data: option(Q.schemaQueryResponse),
    errors: option(array(graphqlError)),
    loading: bool,
    networkStatus: networkStatus,
    error: option(apolloError),
    partial: option(bool),
    stale: option(bool),
  };

  type observer = {
    next: unit => unit,
    error: unit => unit,
  }

  let subscribe: observer => unit = observer => {
    let queryObservable = Belt.Option.getExn(queryObservable^);

    let observerJs: observerJs = {
      "next": () => {
        observer.next()
      },
      "error": () => {
        observer.error()
      },
    };

    subscription := Some(queryObservable##subscribe(observerJs));
  }

  let unsubscribe = () => {
    let subscription = Belt.Option.getExn(subscription^);
    subscription##unsubscribe();
  }

  let currentResult: unit => apolloCurrentQueryResult = () => {
    let queryObservable = Belt.Option.getExn(queryObservable^);

    let result = queryObservable##getCurrentResult();

    {
      data: switch(result##data) {
      | Some(data) => Some(Q.decodeResponse(data))
      | None => None
      },
      errors: switch(result##errors) {
      | Some(errors) => Some(decodeGraphqlErrors(errors, Q.decodeExtension))
      | None => None
      },
      loading: result##loading,
      networkStatus: result##networkStatus->networkStatus_of_int,
      error: switch(result##error) {
      | Some(error) => Some(errorToRecord(error))
      | None => None
      },
      partial: result##partial,
      stale: result##stale,
    }
  }
}