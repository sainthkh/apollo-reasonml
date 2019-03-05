open GraphqlTypes;
open QueryTypes;

module type Config = {
  let client: ApolloClient.apolloClient;
};

module Make = (Config: Config) => {
  module type QueryConfig = {
    type variablesType;
    type schemaQueryResponse;
    let decodeResponse: Js.Json.t => schemaQueryResponse;
    let encodeVariables: variablesType => Js.Json.t;
    type errorExtension;
    let decodeExtension: Js.Json.t => errorExtension;
  };

  module MakeQuery = (Q: QueryConfig) => {
    module QueryObservable = ObservableQuery.Make({
      let client = Config.client;
      
      type schemaQueryResponse = Q.schemaQueryResponse;
      let decodeResponse = Q.decodeResponse;
      type variablesType = Q.variablesType;
      let encodeVariables = Q.encodeVariables;

      type errorExtension = Q.errorExtension;
      let decodeExtension = Q.decodeExtension;
    })

    type queryStatus = 
      | Loading
      | Error(array(QueryObservable.graphqlError))
      | Data(Q.schemaQueryResponse)
    ;

    type response = {
      status: queryStatus,
      data: option(Q.schemaQueryResponse),
      errors: option(array(QueryObservable.graphqlError)),
      networkStatus: networkStatus,
    };

    type state = {
      response: response,
      prevResponse: response,
    };

    type action = 
      | QueryLoaded
      | Update(response)
    ;

    let component:
      ReasonReact.componentSpec(
        state,
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.noRetainedProps,
        action,
      ) = ReasonReact.reducerComponent("Query");

    let initialResponse = {
      status: Loading,
      data: None,
      errors: None,
      networkStatus: Loading,
    };

    let make = (
      ~query: gqlQuery,
      ~variables: option(Q.variablesType),
      ~fetchPolicy: fetchPolicy = CacheFirst,
      ~errorPolicy: errorPolicy = NoPolicy,
      ~pollInterval: int = 0,
      ~notifyOnNetworkStatusChange: bool = false,
      children: array(response => ReasonReact.reactElement),
    ) => {
      ...component,
      initialState: () => {
        response: initialResponse,
        prevResponse: initialResponse,
      },

      didMount: self => {
        QueryObservable.init(
          ~variables=variables,
          ~query=query,
          ~fetchPolicy=fetchPolicy,
          ~errorPolicy=errorPolicy,
          ~pollInterval=pollInterval,
          ~notifyOnNetworkStatusChange=notifyOnNetworkStatusChange,
          ()
        );

        let loaded = () => self.send(QueryLoaded)
        QueryObservable.subscribe({
          next: loaded,
          error: loaded,
        });
      },

      reducer: (action: action, state: state) => {
        switch(action) {
        | QueryLoaded => ReasonReact.SideEffects(self => {
          let result = QueryObservable.currentResult();

          let status = 
            switch(result.loading, result.errors) {
            | (true, _) => Loading
            | (false, None) => Data(Belt.Option.getExn(result.data))
            | (false, Some(_)) => Error(Belt.Option.getExn(result.errors))
            };

          self.send(Update({
            status,
            data: result.data,
            errors: result.errors,
            networkStatus: result.networkStatus,
          }))
        })
        | Update(response) => ReasonReact.Update({ 
            response: response,
            prevResponse: state.response,
          })
        }
      },

      render: self => {
        <>
          {children[0](self.state.response);}
        </>
      }
    }
  }
}
