var Options = require('../').binding.Options;
var Connection = require('../').binding.Connection;
var Query = require('../').binding.Query;

var o = new Options();
o.set('databaseName', 'INNOPAC');
o.set('preferredRecordSyntax', 'usmarc');
o.set('elementSetName', 'F');

var conn = new Connection(o);

conn.connect('192.83.186.170', 210, function (err) {
  if (err) {
    return console.log(err);
  }

  var query = Query().prefix('@attr 1=4 台灣');

  conn.search(query, function (err, resultset) {
    if (err) {
      return console.log(err);
    }
    console.log(resultset.size());
    resultset.getRecords(0, 10, function (err, records) {
      if (err) {
        return console.log(err);
      }
      while (records.hasNext()) {
        var record = records.next();
        console.log(record.get('xml'));
      }
    });
  });
});
