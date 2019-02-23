import React from 'react';
import ReactDOM from 'react-dom';

import {ApolloClient} from 'apollo-client';
import {InMemoryCache} from 'apollo-cache-inmemory';
import {HttpLink} from 'apollo-link-http';
import {Query, ApolloProvider} from 'react-apollo';
import gql from 'graphql-tag';

const cache = new InMemoryCache();
const client = new ApolloClient({
  cache,
  link: new HttpLink({
    uri: 'http://localhost:4000/graphql',
  }),
});

const QUERY = gql`
  query AppQuery {
    hello {
      message
    }
  }
`

ReactDOM.render(
  <ApolloProvider client={client}>
    <Query query={QUERY}>
      {({loading, error, data}) => {
        if (loading) return <div>Loading</div>;
        if (error) {
          console.log(error);
          return <div>Error</div>
        };

        return data.hello 
          ? <div>{data.hello.message}</div>
          : <div>No message</div>
      }}
    </Query>
  </ApolloProvider>,
  document.getElementById('root'),
);
