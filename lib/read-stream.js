'use strict';

var util = require('util');
var Readable = require('stream').Readable;
var Record = require('./record');

module.exports = ReadStream;

util.inherits(ReadStream, Readable);

var stream = ReadStream.prototype;

function ReadStream(conn, options) {
  options || (options = {});
  options.objectMode = true;
  Readable.call(this, options);

  this._zoomState = {
    conn: conn,
    index: 0,
    start: options.start | 0,
    chunk: (options.chunk || 20) | 0,
    limit: options.limit | 0,
    total: 0,
    resultset: null,
    records: null,
    waiting: false,
    destroyed: false
  };
}

stream._read = function () {
  var state = this._zoomState;

  if (state.destroyed) {
    return;
  }

  if (!state.conn._connected) {
    this._connect();
    return state.waiting = true;
  }

  if (!state.resultset) {
    this._getResultset();
    return state.waiting = true;
  }

  if (!(state.records && state.records.hasNext())) {
    this._moreRecords();
    return state.waiting = true;
  }

  var record = state.records.next();
  this.push(record && new Record(record));

  state.index += 1;

  if ((state.start + state.index) >= state.total
    || state.index >= state.limit) {
    this.push(null);
    this.destroy();
  }
};

stream.destroy = function () {
  var state = this._zoomState;
  if (state && !state.destroyed) {
    state.destroyed = true;
    delete this._zoomState;
    this.emit('close');
  }

  return this;
};

stream._getResultset = function () {
  var state = this._zoomState;
  var conn = state.conn;

  conn._conn.search(state.conn._query, function (err, resultset) {
    if (err) {
      this.emit('error', err);
      this.destroy();
      return;
    }
    state.resultset = resultset;
    state.total = resultset.size();
    this._zoomReady();
  }.bind(this));
};

stream._moreRecords = function () {
  var state = this._zoomState;
  var resultset = state.resultset;
  var start = state.start + state.index;
  var count = state.chunk;

  resultset.getRecords(start, count, function (err, records) {
    if (err) {
      this.emit('error', err);
      this.destroy();
      return;
    }
    state.records = records;
    this._zoomReady();
  }.bind(this));
};

stream._zoomReady = function () {
  var state = this._zoomState;

  if (state.waiting && !state.destroyed) {
    state.waiting = false;
    this._read();
  }
};

stream._connect = function () {
  var state = this._zoomState;

  state.conn.connect(function (err) {
    if (err) {
      this.emit('error', err);
      this.destroy();
      return;
    }
    this._zoomReady();
  }.bind(this));
};
