module type Config = {
  let client: JsImport.apolloClient;
};

module Make = (Config: Config) => {
  module type QueryConfig = {
    type variablesType;
    type schemaQueryResponse;
    let decodeResponse: Js.Json.t => schemaQueryResponse;
  };

  module MakeQuery = (Q: QueryConfig) => {
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
      ~variables: Q.variablesType,
      ~render: response => ReasonReact.reactElement,
      _children
    ) => {
      ...component,
      initialState: () => {
        data: None,
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

/*
module Make2 = (Q: QueryConfig) => {
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
    errors: array(graphqlError),
  };

  type state = {
    data: option(Q.schemaQueryResponse),
  };

  type action = Dummy;

  let component = ReasonReact.reducerComponent("");

  let make = (
    ~render: response => ReasonReact.reactElement,
    _children
  ) => {
    ...component,
    initialState: () => {
      data: None,
    },

    reducer: (action: action, _state: state) => {
      switch(action) {
      | Dummy => ReasonReact.Update({ data: None })
      }
    },

    render: _self => {
      <>
        { render({
          status: Loading,
          data: None,
          errors: [||],
        })}
      </>
    }
  }
}
*/