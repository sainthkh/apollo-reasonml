open ApolloLink;
open ApolloClient;

let createInMemoryCache = ApolloCache.createInMemoryCache;
type gqlQuery = GraphqlTypes.gqlQuery;
let gql = GraphqlTypes.gql;

let createHttpLink = (
  ~uri=?,
  ~includeExtensions=?,
  ~fetch=?,
  ~headers=?,
  ~credentials=?,
  ~fetchOptions=?,
  ()
) => {
  createHttpLinkJs(
    linkOptions(
      ~uri=uri,
      ~includeExtensions=includeExtensions,
      ~fetch=fetch,
      ~headers=headers,
      ~credentials=credentials,
      ~fetchOptions=fetchOptions
    )
  );
}

let createUploadLink = (
  ~uri=?,
  ~includeExtensions=?,
  ~fetch=?,
  ~headers=?,
  ~credentials=?,
  ~fetchOptions=?,
  ()
) => {
  createHttpLinkJs(
    linkOptions(
      ~uri=uri,
      ~includeExtensions=includeExtensions,
      ~fetch=fetch,
      ~headers=headers,
      ~credentials=credentials,
      ~fetchOptions=fetchOptions
    )
  );
}

let clientOptions = (
  ~link,
  ~cache,
  ~connectToDevTools=?,
  ~queryDuplication=?,
  ()
) => {
  ApolloClient.apolloClientOptions(
    ~link=link,
    ~cache=cache,
    ~connectToDevTools=connectToDevTools,
    ~queryDuplication=queryDuplication,
  )
};

module type ApolloConfig = {
  let clientOptions: apolloClientOptions;
};

module Init = (Config: ApolloConfig) => {
  let client = createApolloClientJS(Config.clientOptions);

  module Component = Apollo__Components.Make({
    let client = client;
  });

  module Query = Component.MakeQuery;
}