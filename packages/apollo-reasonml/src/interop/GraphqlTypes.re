type sourceLocation = Js.t({.
  line: int,
  column: int,
});

type gqlSource = Js.t({.
  body: string,
  name: string,
  locationOffset: sourceLocation,
});

type graphqlErrorJs = Js.t({.
  message: string,
  locations: option(array(sourceLocation)),
  path: option(array(string)),
  source: option(gqlSource),
  positions: option(array(int)),
  extensions: option(Js.Json.t),
});

let decodeGraphqlErrors = (errors, decodeExtension) => {
  [||]
}

type gqlQuery;
[@bs.module "graphql-tag"]
external gql: string => gqlQuery = "default";