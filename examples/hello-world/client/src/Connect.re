let httpLink = Apollo.createHttpLink(
  ~uri="http://localhost:4000/graphql", 
  ()
);
let cache = Apollo.createInMemoryCache();

module Apollo = Apollo.Init({
  let clientOptions = Apollo.clientOptions(
    ~link=httpLink,
    ~cache=cache,
    ()
  );
});
