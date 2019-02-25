type apolloCache;

[@bs.module "apollo-cache-inmemory"]
[@bs.new]
external createInMemoryCache : 'a => apolloCache = "InMemoryCache";

type apolloLink;
type fetch;

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

type apolloClient;

[@bs.deriving abstract]
type apolloClientOptions = {
  link: apolloLink,
  cache: apolloCache,
  ssrMode: option(bool),
  ssrForceFetchDelay: option(int),
  connectToDevTools: option(bool),
  queryDuplication: option(bool),
};

[@bs.module "apollo-client"] [@bs.new]
external createApolloClientJS: 'a => apolloClient = "ApolloClient";

[@bs.module "react-apollo"]
external apolloProvider : ReasonReact.reactClass = "ApolloProvider";