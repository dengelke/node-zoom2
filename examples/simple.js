'use strict';

var zoom = require('../lib');

zoom.connection('192.83.186.170:210/INNOPAC')
  .set('preferredRecordSyntax', 'usmarc')
  .query('prefix', '@attr 1=7 ' + '9780073383095')
  .search(function (err, resultset) {
    resultset.getRecords(0, resultset.size, function (err, records) {
      while (records && records.hasNext()) {
        var record = records.next();
        console.log(record);
      }
    });
  });
