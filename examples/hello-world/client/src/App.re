let component = ReasonReact.statelessComponent("App")

let query = Apollo.gql({|
  query AppQuery {
    hello {
      message
    }
  }
|})

let make = (_children) => {
  ...component,

  render: _self => {
    <div>

    </div>
  }
}
