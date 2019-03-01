type fetchPolicy =
  | CacheFirst
  | CacheAndNetwork
  | NetworkOnly
  | CacheOnly
  | NoCache
  | StandBy

let string_of_fetchPolicy = policy => 
  switch(policy) {
  | CacheFirst => "cache-first"
  | CacheAndNetwork => "cache-and-network"
  | NetworkOnly => "network-only"
  | CacheOnly => "cache-only"
  | NoCache => "no-cache"
  | StandBy => "stand-by"
  }

type errorPolicy = 
  | NoPolicy
  | All
  | Ignore

let string_of_errorPolicy = policy => 
  switch(policy) {
  | NoPolicy => "none"
  | All => "all"
  | Ignore => "ignore"
  };

type gqlQuery;
type apolloCache;
type apolloLink;
type fetch;

[@bs.deriving abstract]
type observableQueryOpts = {
  variables: option(Js.Json.t),
  fetchPolicy: string,
  errorPolicy: string,
  query: gqlQuery,
  pollInterval: int,
  notifyOnNetworkStatusChange: bool,
};

type observableQuery;
type apolloClient = Js.t({.
  [@bs.meth] watchQuery: observableQueryOpts => observableQuery,
});


[@bs.module "apollo-cache-inmemory"]
[@bs.new]
external createInMemoryCache : 'a => apolloCache = "InMemoryCache";

[@bs.deriving abstract]
type linkOptions = {
  uri: option(string),
  includeExtensions: option(bool),
  fetch: option(fetch),
  headers: option(Js.Json.t),
  credentials: option(string),
  fetchOptions: option(Js.Json.t),
};

[@bs.module "apollo-link-http"] [@bs.new]
external createHttpLinkJs: linkOptions => apolloLink = "HttpLink";

[@bs.module "apollo-upload-client"]
external createUploadLinkJs: linkOptions => apolloLink = "createUploadLink";

[@bs.deriving abstract]
type apolloClientOptions = {
  link: apolloLink,
  cache: apolloCache,
  connectToDevTools: option(bool),
  queryDuplication: option(bool),
  /* TODO: support other options. */
};

[@bs.module "apollo-client"] [@bs.new]
external createApolloClientJS: 'a => apolloClient = "ApolloClient";

[@bs.module "graphql-tag"]
external gql: string => gqlQuery = "default";