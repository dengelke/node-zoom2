'use strict';

var zoom = require('../lib');

zoom.connection('192.83.186.170:210/INNOPAC')
.set('preferredRecordSyntax', 'usmarc')
.query('prefix', '@attr 1=7 ' + '9780073383095')
.createReadStream()
.on('data', function (record) {
  console.log(record.json, record.xml, record.raw);
})
.on('close', function(close) {
  process.exit(1);
})