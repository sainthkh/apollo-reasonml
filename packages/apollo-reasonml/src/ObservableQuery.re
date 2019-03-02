open GraphqlTypes;
open QueryTypes;

module type QueryConfig = {
  let client: ApolloClient.apolloClient;

  type variablesType;
  let encodeVariables: variablesType => Js.Json.t;
};

module Make = (Q: QueryConfig) => {
  let queryObservable: Pervasives.ref(
    option(observableQuery)
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

  let subscribe: observer => subscription = observer => {
    let Some(queryObservable) = queryObservable^;
    queryObservable##subscribe(observer);
  }
}