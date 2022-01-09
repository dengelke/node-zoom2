var zoom = require('../lib');

var expect = require('chai').expect;

describe('connection', function() {
  this.timeout(20000);
  it('should get/set values', function() {
    expect(zoom.connection('192.83.186.170:210/INNOPAC')
    .set('preferredRecordSyntax', 'usmarc')
    .get('preferredRecordSyntax')).to.equal('usmarc');
  })
  it('should set query with only 1 one arguement', function() {
    expect(zoom.connection('192.83.186.170:210/INNOPAC')
    .set('preferredRecordSyntax', 'usmarc')
    .query('@attr 1=7 ' + '9780073383095')._query['prefix']).to.be.an('function');
  })
  it('should set query with only 1 one arguement', function() {
    expect(zoom.connection('192.83.186.170:210/INNOPAC')
    .set('preferredRecordSyntax', 'usmarc')
    .query('@attr 1=4 ' + 'Jitter in')._query['prefix']).to.be.an('function');
  })
  it('should fail at createReadStream without query', function(done) {
    try {
      zoom.connection('192.83.186.170:210/INNOPAC')
      .set('preferredRecordSyntax', 'usmarc')
      .createReadStream()
    } catch (e) {
      if (e.message.indexOf('Query not found') !== -1) { 
        done() 
      } else {
        done(e)
      }
    }
  })
  it('should fail at searching without query', function(done) {
    try {
      zoom.connection('192.83.186.170:210/INNOPAC')
      .set('preferredRecordSyntax', 'usmarc')
      .search()
    } catch (e) {
      if (e.message.indexOf('Query not found') !== -1) { 
        done() 
      } else {
        done(e)
      }
    }
  })
  it('should fail at sorting without query', function(done) {
    try {
      zoom.connection('192.83.186.170:210/INNOPAC')
      .set('preferredRecordSyntax', 'usmarc')
      .sort('error')
    } catch (e) {
      if (e.message.indexOf('Query not found') !== -1) { 
        done() 
      } else {
        done(e)
      }
    }
  })
  it('should have error', function(done) {
    zoom.connection('192.83.186.170:210/INNOPA')
    .set('preferredRecordSyntax', 'usmarc')
    .query('prefix', '@attr 1=7 ' + '9780230343719')
    .search(function (err, resultset) {
      if (err) {
        done();
      } else {
        done('should be an error');
      }
    })
  })
  it('should query, search and resultset functions work', function(done) {
    zoom.connection('192.83.186.170:210/INNOPAC')
    .set('preferredRecordSyntax', 'usmarc')
    .query('prefix', '@attr 1=7 ' + '9780073383095')
    .search(function (err, resultset) {
      expect(resultset.size).to.be.greaterThan(0);
      resultset.getRecords(0, 10, function (err, records) {
        while (records && records.hasNext()) {
          var record = records.next();
          if (record.json) {
            break;
          }
        }
        if (record.json) {
          resultset.set('test', 'value');
          expect(resultset.get('test')).to.equal('value');
          done();
        } else {
          done('No record found')
        }
      })
    })
  })
  it('should createReadStream', function(done) {
    zoom.connection('192.83.186.170:210/INNOPAC')
    .set('preferredRecordSyntax', 'usmarc')
    .query('prefix', '@attr 1=7 ' + '9780073383095')
    .createReadStream({chunk: 1, limit: 1})
    .on('data', function (record) {
      if (record.json && record.xml && record.txml && record.render && record.raw) {
        expect(record.schema).to.be.an('undefined');
        expect(record.database).to.equal('INNOPAC');
        expect(record.syntax).to.equal('USmarc');
        done();
      }
    })
  })
})