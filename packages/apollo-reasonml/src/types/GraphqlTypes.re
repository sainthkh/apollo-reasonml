type sourceLocation = Js.t({.
  line: int,
  column: int,
});

type gqlSource = Js.t({.
  body: string,
  name: string,
  locationOffset: sourceLocation,
});

type graphqlError = Js.t({.
  message: string,
  locations: option(array(sourceLocation)),
  path: option(array(string)),
  nodes: option(array(Js.Json.t)),
  source: option(gqlSource),
  positions: option(array(int)),
});

type gqlQuery;

[@bs.module "graphql-tag"]
external gql: string => gqlQuery = "default";