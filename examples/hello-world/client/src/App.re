open AppQuery;

let component = ReasonReact.statelessComponent("App")

let query = Apollo.gql({|
  query AppQuery {
    hello {
      message
    }
  }
|})

module Query = Connect.Apollo.Query(AppQuery);

let make = (_children) => {
  ...component,

  render: _self => {
    <Query
      variables=encodeVariables()
      render=(_response => {
        <div>
          { ReasonReact.string("Hello") }
        </div>
      })
    />
  }
}
