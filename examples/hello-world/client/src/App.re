open AppQuery;

let component = ReasonReact.statelessComponent("App")

let query = Apollo.gql({|
  query AppQuery {
    hello {
      message
    }
  }
|})

module Query = Create.Apollo.Query(AppQuery);

let make = (_children) => {
  ...component,

  render: _self => {
    <Query
      query
      variables=None
    >
      {_response => {
        <div>
          { ReasonReact.string("Hello") }
        </div>
      }}
    </Query>
  }
}
