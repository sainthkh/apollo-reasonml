open QueryTypes;
open ApolloLink;
open ApolloCache;

type apolloClient = Js.t({.
  [@bs.meth] watchQuery: observableQueryOpts => observableQuery,
});

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