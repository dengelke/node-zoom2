'use strict';

module.exports = Record;

function Record(record) {
  this._record = record;
}

Record.prototype = {
  get: function (type) {
    return this._record.get(type);
  },

  get json() {
    return JSON.parse(this.get('json'));
  },

  get database() {
    return this.get('database');
  },
  
  get syntax() {
    return this.get('syntax');
  },

  get schema() {
    return this.get('schema');
  },

  get render() {
    return this.get('render');
  },

  get raw() {
    return this.get('raw');
  },

  get xml() {
    return this.get('xml');
  },

  get txml() {
    return this.get('txml');
  }
};
