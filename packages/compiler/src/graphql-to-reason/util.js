function commentOnTop() {
  return '/* Generated by Reason Relay Compiler, PLEASE EDIT WITH CARE */'
}

function generateTypeCode(typeList) {
  return typeList.map(type => {
    let name = lowerTheFirstCharacter(type.name);
    name = handleRootNames(name);
    return `
type ${name} = {
${
  type.fields.map(field => {
    return `  ${field.name}: ${decodeTypeName(field)},`
  }).join('\n')
}
};
`.trim();
  }).join('\n\n');
}

function handleRootNames(name) {
  let rootNames = {
    "query": "queryResponse",
    "mutation": "mutationResponse",
    "subscription": "subscriptionResponse",
  };

  let rootName = rootNames[name];

  return rootName ? rootName : name;
}

function decodeTypeName(field) {
  let typeNames = {
    "ID": "string",
    "String": "string",
    "Boolean": "bool",
    "Int": "int",
    "Float": "float",
  };

  let typeName = typeNames[field.type];
  typeName = typeName ? typeName : lowerTheFirstCharacter(field.type);
  typeName = field.contentOption
    ? `option(${typeName})`
    : typeName;
  
  typeName = field.array
    ? `array(${typeName})`
    : typeName;

  return field.option? `option(${typeName})` : typeName;
}

function lowerTheFirstCharacter(name) {
  return name[0].toLowerCase() + name.substring(1);
}

function childTypes(types, type) {
  let typeList = []
  type.fields.reverse().forEach(field => {
    if(!isScalar(field.type)){
      typeList = childTypes(types, types[field.type]).concat(typeList);
    }
  });
  type.fields.reverse();
  if(!type.inList) {
    typeList.push(type);
    type.inList = true;
  }
  return typeList;
}

function isScalar(type) {
  let scalarTypes = ["ID", "String", "Int", "Float", "Boolean"];
  return scalarTypes.includes(type);
}

function handleRootTypeNames(name) {
  let names = {
    "Query": "QueryResponse",
    "Mutation": "MutationResponse",
    "Subscription": "SubscriptionResponse",
  };

  let typeName = names[name];
  return typeName ? typeName : name;
}

module.exports = {
  generateTypeCode,
  commentOnTop,
  childTypes,
  isScalar,
  handleRootTypeNames,
}