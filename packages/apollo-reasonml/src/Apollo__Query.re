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
  };

  module MakeQuery = (Q: QueryConfig) => {
    module QueryObservable = ObservableQuery.Make({
      let client = Config.client;
      
      type variablesType = Q.variablesType;
      let encodeVariables = Q.encodeVariables;
    })

    type queryStatus = 
      | Loading
      | Error
      | Data
    ;

    type graphqlError = {
      message: string,
    };

    type response = {
      status: queryStatus,
      data: option(Q.schemaQueryResponse),
      errors: option(array(graphqlError)),
    };

    type state = {
      data: option(Q.schemaQueryResponse),
    };

    type action = 
      | Fetch
      | Update
    ;

    let component:
      ReasonReact.componentSpec(
        state,
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.noRetainedProps,
        action,
      ) = ReasonReact.reducerComponent("Query");

    let make = (
      ~query: gqlQuery,
      ~variables: option(Q.variablesType),
      ~fetchPolicy: fetchPolicy = CacheFirst,
      ~errorPolicy: errorPolicy = NoPolicy,
      ~pollInterval: int = 0,
      ~notifyOnNetworkStatusChange: bool = false,
      ~render: response => ReasonReact.reactElement,
      _children
    ) => {
      ...component,
      initialState: () => {
        data: None,
      },

      didMount: _self => {
        QueryObservable.init(
          ~variables=variables,
          ~query=query,
          ~fetchPolicy=fetchPolicy,
          ~errorPolicy=errorPolicy,
          ~pollInterval=pollInterval,
          ~notifyOnNetworkStatusChange=notifyOnNetworkStatusChange,
          ()
        );
      },

      reducer: (action: action, _state: state) => {
        switch(action) {
        | Fetch => ReasonReact.Update({ data: None })
        | Update => ReasonReact.Update({ data: None })
        }
      },

      render: _self => {
        <>
          { render({
            status: Loading,
            data: None,
            errors: None,
          })}
        </>
      }
    }
  }
}
