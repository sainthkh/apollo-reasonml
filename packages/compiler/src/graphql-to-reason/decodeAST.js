function decodeField(field) {
  switch(field.type.kind) {
  case "NamedType": 
    return {
      name: field.name.value,
      type: field.type.name.value,
      option: true,
    }
  case "NonNullType": 
    if(field.type.type.kind != "ListType") {
      return {
        name: field.name.value,
        type: field.type.type.name.value,
        option: false,
      }
    } else {
      let nullable = field.type.type.type.kind == "NamedType";
      return {
        name: field.name.value, 
        type: nullable
          ? field.type.type.type.name.value
          : field.type.type.type.type.name.value,
        option: false,
        array: true,
        contentOption: nullable,
      }
    }
  case "ListType":
    let nullableType = field.type.type.kind == "NamedType";
    return {
      name: field.name.value,
      type: nullableType
        ? field.type.type.name.value
        : field.type.type.type.name.value,
      option: true,
      array: true,
      contentOption: nullableType,
    }
  }
}

exports.decodeField = decodeField;