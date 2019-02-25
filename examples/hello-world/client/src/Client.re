let httpLink = Apollo.createHttpLink(
  ~uri="http://localhost:4000/graphql", 
  ()
);
let cache = Apollo.createInMemoryCache();
let client = Apollo.createApolloClient(
  ~link=httpLink,
  ~cache=cache,
  ()
);