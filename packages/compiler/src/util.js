const glob = require('fast-glob');
const argv = require('yargs').argv;
const isUrl = require('is-url');
const chokidar = require('chokidar');

const fs = require('fs');
const path = require('path');

function loadConfig() {
  const configPath = path.join(process.cwd(), 'apollore.config.js');
  if(!fs.existsSync(configPath)) {
    console.log(`apollore.config.js file doesn't exist.`)
    return;
  }

  let conf = require(configPath);
  conf = Object.assign({
    schema: '',
    localschema: '',
    src: './src',
    include: '**',
    exclude: [
      '**/node_modules/**', 
      '**/__mocks__/**', 
      '**/__tests__/**', 
      '**/.*/**',
    ],
    watch: false,
  }, conf);

  if(argv.w || argv.watch) {
    conf.watch = true;
  }

  return conf;
}

function loadServerSchema({ schema }) {
  let ast = {};

  console.log('loading schema...');
  if(isUrl(schema)) {
    console.log('fetching schema from server...');
  } else if(schema) {
    console.log('parsing the schema file...');

    const schemaPath = path.join(process.pwd(), schema);
    if(fs.existsSync(schemaPath)) {

    } else {
      console.log(`GraphQL schema file doesn't exist at ${schemaPath}`);
    }
  } else {
    console.log('"schema" field should not be empty in apollore.config.js');
  }

  return ast;
}

function extendWithClientSchema({client}, ast) {
  return ast;
}


function generateSchemaTypes(ast) {

}

function generateTypeFiles({include, exclude, watch, src}, ast) {
  include = Array.isArray(include) ? include : [include];
  const patterns = include.map(inc => `${inc}/*.re`);

  if(!watch) {
    const files = glob.sync(patterns, {
      cwd: src,
      ignore: exclude,
    });
    console.log(files);
  }
}

module.exports = {
  loadConfig,
  loadServerSchema,
  extendWithClientSchema,
  generateSchemaTypes,
  generateTypeFiles,
}