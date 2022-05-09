'use strict';

module.exports = Record;

function Record(record) {
  this._record = record;
}

Record.prototype = {
  get: function (type) {
    return this._record ? this._record.get(type) : undefined;
  },

  get json() {
    const json = this.get('json');
    return json !== undefined ? JSON.parse(json) : undefined;
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
