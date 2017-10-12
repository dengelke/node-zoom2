'use strict';

var Record = require('./record');

module.exports = Records;

var records = Records.prototype;

function Records(records) {
  this._records = records;
}

records.hasNext = function () {
  return this._records.hasNext();
};

records.next = function () {
  var record = this._records.next();
  return new Record(record);
};
