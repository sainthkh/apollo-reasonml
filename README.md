# Another give up. 

A few days ago, I gave up creating [reason-relay](https://github.com/sainthkh/reason-relay) and this time I gave up apollo-reasonml. 

You might think I have a chronic disease of not getting things done. (Maybe, you're right.)

But Apollo client and its family features made me think again about using Apollo client as the base for the GraphQL library for ReasonML. 

## Is there a universal cache system for all ReasonML apps? 

It was the first question that came to my mind after re-inventing the Apollo wheels. 

Apollo has a cache system and it's their marketing point. But after researching it, there is a problem. For example, mutation can invalidate the cache. (i.e. Let's say you showed a list of todos with a query and changed it with mutation. If todo type in query and mutations requires different number of fields, it can cause failure. That's why we need [`partialRefetch`](https://github.com/apollographql/react-apollo/issues/2576) in `<Query>` component.)

Although it's a bit burdensome, [micro-graphql-react](https://github.com/arackaf/micro-graphql-react) way of manual handling cache is less buggy and efficient. 

## Apollo client is a too big. 

Currently in March 2019, bundled size of [Apollo client is 40kb](https://github.com/apollographql/apollo-client/issues/4224). 

It got this bigger, because it has many features like cache, link, state, etc. 

But after reading all those codes, it made me think. Are they necessary? Yes. They are. But partially. 

Sometimes, we need this. Sometimes, we need that. We don't need all of them every time. 

So, it made me think. How about starting from scratch with minial use cases? That became my goal. And started [another project](https://github.com/sainthkh/reason-graphql). 

## It doesn't mean I'm entirely leaving from Apollo. 

The problems are in Apollo Client, not in Apollo Server. So, I'll use Apollo Server as the primary backend. 