module type Config = {
  let client: Apollo__Types.apolloClient;
};

module Make = (Config: Config) => {
  module InternalQuery = Apollo__Query.Make({
    let client = Config.client;
  })

  module MakeQuery = InternalQuery.MakeQuery;
}