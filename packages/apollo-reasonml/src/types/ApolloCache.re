type apolloCache;

[@bs.module "apollo-cache-inmemory"]
[@bs.new]
external createInMemoryCache : unit => apolloCache = "InMemoryCache";
