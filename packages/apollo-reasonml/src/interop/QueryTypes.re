open GraphqlTypes;

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

type networkStatus = 
  | Loading
  | SetVariables
  | FetchMore
  | Refetch
  | Poll
  | NetworkReady
  | NetworkError

let int_of_networkStatus = status => 
  switch(status) {
  | Loading => 1
  | SetVariables => 2
  | FetchMore => 3
  | Refetch => 4
  | Poll => 6
  | NetworkReady => 7
  | NetworkError => 8
  };

let networkStatus_of_int = num => 
  switch(num) {
  | 1 => Loading
  | 2 => SetVariables
  | 3 => FetchMore
  | 4 => Refetch
  | 6 => Poll
  | 7 => NetworkReady
  | 8 | _ => NetworkError
  };

[@bs.deriving abstract]
type observableQueryOpts = {
  variables: option(Js.Json.t),
  fetchPolicy: string,
  errorPolicy: string,
  query: gqlQuery,
  pollInterval: int,
  notifyOnNetworkStatusChange: bool,
};

type apolloQueryResult = Js.t({.
  data: Js.Json.t,
  errors: Js.Nullable.t(array(graphqlError)),
  loading: bool,
  networkStatus: int,
});

type apolloError = Js.t({.
  message: string,
  grqphqlErrors: array(graphqlError),
  networkError: Js.Nullable.t(Js.Exn.t),
});

type observer = {
  next: apolloQueryResult => unit,
  error: apolloError => unit,
};

type subscription = Js.t({.
  unsubscribe: unit => unit,
});

type observableQuery = Js.t({.
  [@bs.meth] subscribe: observer => subscription
});