const glob = require('fast-glob');
const argv = require('yargs').argv;
const isUrl = require('is-url');
const chokidar = require('chokidar');
const {parse} = require('graphql');
const {schemaToReason} = require('./graphql-to-reason/schema');

const fs = require('fs-extra');
const path = require('path');

function loadConfig() {
  const configPath = path.join(process.cwd(), 'apollore.config.js');
  if(!fs.existsSync(configPath)) {
    console.log(`apollore.config.js file doesn't exist.`)
    process.exit();
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
    // TODO: Should support later. Because it's more complicated than expected. 
    // Check 
    // https://github.com/graphql/graphql-js/blob/master/src/type/introspection.js and 
    // https://github.com/mhallin/graphql_ppx/blob/master/sendIntrospectionQuery.js
  } else if(schema) {
    console.log('parsing the schema file...');

    const schemaPath = path.join(process.cwd(), schema);
    if(fs.existsSync(schemaPath)) {
      let ext = path.extname(schemaPath);
      let code = '';
      if(ext == '.js') {
        code = fs.readFileSync(schemaPath).toString();
        let match = code.match(/gql`([\s\S]+)`/);
        if(match) {
          code = match[1];
        } else {
          console.log(`The file ${schema} doesn't have graphql tagged template literal "gql".`);
          process.exit();
        }
      } else if (ext == '.graphql' || ext == '.gql') {
        code = fs.readFileSync(schemaPath).toString();
      } else {
        console.log('The extension of server schema files should be .js, .graphql, .gql.');
        process.exit();
      }
      ast = parse(code);
    } else {
      console.log(`GraphQL schema file doesn't exist at ${schemaPath}`);
    }
  } else {
    console.log('"schema" field is required in apollore.config.js');
  }

  return ast;
}

function extendWithClientSchema({client}, ast) {
  return ast;
}

const DEST_DIR = './src/.apollo';

function generateSchemaTypes(ast) {
  let {reason, codec} = schemaToReason(ast);

  fs.ensureDirSync(DEST_DIR);
  fs.writeFileSync(path.join(DEST_DIR, 'SchemaTypes.re'), reason);
  fs.writeFileSync(path.join(DEST_DIR, 'SchemaTypes.codec.js'), codec);
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