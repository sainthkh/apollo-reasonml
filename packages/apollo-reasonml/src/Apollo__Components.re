module type Config = {
  let client: JsImport.apolloClient;
};

module Make = (Config: Config) => {
  module InternalQuery = Apollo__Query.Make({
    let client = Config.client;
  })

  module MakeQuery = InternalQuery.MakeQuery;
}