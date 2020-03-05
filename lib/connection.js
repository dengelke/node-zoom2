'use strict';

var Query_ = require('./binding').Query;
var Options_ = require('./binding').Options;
var Connection_ = require('./binding').Connection;
var noop = require('./noop');
var ResultSet = require('./resultset');
var ReadStream = require('./read-stream');

module.exports = Connection;

var conn = Connection.prototype;

function Connection(host) {
  if (!(this instanceof Connection)) {
    return new Connection(host);
  }

  this._connected = false;
  this._options = Options_();
  this._conn = new Connection_(this._options);
  this.set('implementationName', 'node-zoom2');

  var parsed = this._parseHost(host || '');
  parsed.database && this.set('databaseName', parsed.database);
  this._host = parsed.host;
  this._port = parsed.port | 0;
}

conn.set = function (key, val) {
  this._options.set(key, val);
  return this;
};

conn.get = function (key) {
  return this._options.get(key);
};

conn.query = function (type, queryString) {
  if (arguments.length < 2) {
    queryString = type;
    type = 'prefix';
  }
  
  // Fix for spaces in query string
  var queryArray = queryString.split(' ');
  queryString = [queryArray.slice(0,2).join(' '), queryArray.slice(2).join('\u00A0')].join(' ');

  var clone = Object.create(this);
  clone._query = Query_();
  if (!clone._query[type]) {
    throw new Error('Unknown query type');
  }

  clone._query[type](queryString);

  return clone;
};

conn.updateRecord = function(options, cb) {

  if (!options) {
      throw new Error('No package options given');
  }
  
  var packageOpts = Options_();
  Object.keys(options)
    .forEach(function(k) {
      packageOpts.set(k, options[k])
    });

  cb || (cb = noop);

  this.connect(function (err) {
    if (err) {
      cb(err);
      return;
    }

    this._conn.update(packageOpts, cb);
  }.bind(this));
  
}

conn.sort = function () {
  if (!this._query) {
    throw new Error('Query not found');
  }
  this._query.sortBy.apply(this._query, arguments);
  return this;
};

conn.connect = function (cb) {
  cb || (cb = noop);

  if (this._connected) {
    cb(null);
  } else {
    this._conn.connect(this._host, this._port, function (err) {
      this._connected = !err;
      cb(err);
    }.bind(this));
  }

  return this;
};

conn.createReadStream = function (options) {
  if (!this._query) {
    throw new Error('Query not found');
  }
  return new ReadStream(this, options);
};

conn.search = function (cb) {
  if (!this._query) {
    throw new Error('Query not found');
  }

  cb || (cb = noop);

  this.connect(function (err) {
    if (err) {
      cb(err);
      return;
    }

    this._conn.search(this._query, function (err, resultset) {
      if (err) {
        cb(err);
        return;
      }
      cb(null, new ResultSet(resultset));
    }.bind(this));
  }.bind(this));

  return this;
};

conn._parseHost = function (host) {
  var match = host.match(/^(.+?)(?::(\d+))?(?:\/(.+))?$/);
  return match ? {
    host: match[1],
    port: match[2],
    database: match[3]
  } : {};
};

function unicodeEscape(str) {
  return str.replace(/[\s\S]/g, function (escape) {
    return '\\u' + ('0000' + escape.charCodeAt().toString(16)).slice(-4);
  });
}