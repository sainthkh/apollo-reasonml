open JsImport;

let createInMemoryCache = JsImport.createInMemoryCache;

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

let createApolloClient = (
  ~link,
  ~cache,
  ~ssrMode=?,
  ~ssrForceFetchDelay=?,
  ~connectToDevTools=?,
  ~queryDuplication=?,
  ()
) => {
  createApolloClientJS(
    apolloClientOptions(
      ~link=link,
      ~cache=cache,
      ~ssrMode=ssrMode,
      ~ssrForceFetchDelay=ssrForceFetchDelay,
      ~connectToDevTools=connectToDevTools,
      ~queryDuplication=queryDuplication
    )
  );
}