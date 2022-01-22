'use strict';

var noop = require('./noop');
var Records = require('./records');

module.exports = ResultSet;

function ResultSet(resultset) {
  this._resultset = resultset;
}

ResultSet.prototype = {
  set: function (key, val) {
    this._resultset.setOption(key, val);
    return this;
  },

  get: function (key) {
    return this._resultset.getOption(key);
  },

  get size() {
    return this._resultset.size();
  },

  destroy: function() {
    this._resultset.destroy();
    delete this._resultset;
  },

  getRecords: function (index, counts, cb) {
    cb || (cb = noop);
    this._resultset.getRecords(index, counts, function (err, records) {
      if (err) {
        cb(err);
        return;
      }
      cb(null, new Records(records));
    });
  }
};
