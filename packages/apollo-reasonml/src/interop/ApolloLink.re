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
