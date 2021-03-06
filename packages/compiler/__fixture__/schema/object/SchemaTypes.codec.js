/* Generated by Reason Relay Compiler, PLEASE EDIT WITH CARE */

var decodeB = function (res) {
  return [
    res.id,
    res.iii,
  ]
}

var decodeA = function (res) {
  return [
    res.id,
    res.ii,
    decodeB(res.b),
    res.ff,
  ]
}

var decodeC = function (res) {
  return [
    res.id,
    res.ss,
  ]
}

var decodeQueryResponse = function (res) {
  return [
    res.i,
    res.a ? decodeA(res.a) : undefined,
    res.s,
    decodeC(res.c),
  ]
}

exports.decodeB = decodeB;
exports.decodeA = decodeA;
exports.decodeC = decodeC;
exports.decodeQueryResponse = decodeQueryResponse;