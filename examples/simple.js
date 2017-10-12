'use strict';

var zoom = require('../lib');

zoom.connection('192.83.186.170:210/INNOPAC')
  .set('preferredRecordSyntax', 'usmarc')
  .query('prefix', '@attr 1=4 台灣')
  .search(function (err, resultset) {
    // resultset.set('option name', 'option value');
    resultset.getRecords(0, 10, function (err, records) {
      while (records.hasNext()) {
        console.log(records.next().json);
      }
    });
  });
