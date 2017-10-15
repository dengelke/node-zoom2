'use strict';

var expect = require('chai').expect;
var Options = require('..').binding.Options;

describe('Options', function () {

  describe('constructor()', function () {
    it('should work', function () {
      var opts = new Options();
    });
  });

  describe('constructor(opts)', function () {
    var opts;

    before(function () {
      opts = new Options();
    });

    it('should work', function () {
      var opts2 = new Options(opts);
    });
  });

  describe('#set(kev, val)', function () {
    var opts;

    before(function () {
      opts = new Options();
    });

    it('should work', function () {
      opts.set('abc', 123);
      opts.set('abc', '123');
      opts.set('abc', null);
      opts.set('abc', undefined);
    });

    it('should fail', function () {
      expect(function () {
        opts.set();
      }).to.throw(Error);

      expect(function () {
        opts.set(123, 123);
      }).to.throw(Error);

      expect(function () {
        opts.set(null, 123);
      }).to.throw(Error);
    });
  });

  describe('#get(key)', function () {
    var opts;

    before(function () {
      opts = new Options();
      opts.set('num', 123);
      opts.set('null', null);
      opts.set('undefined', undefined);
    });

    it('should work', function () {
      expect(opts.get('num')).to.equal('123');
      expect(opts.get('null')).to.equal('null');
      expect(opts.get('undefined')).to.equal('undefined');
    });

    it('should get undefined', function () {
      expect(opts.get('aaa')).to.be.an('undefined');
      expect(opts.get('bbb')).to.be.an('undefined');
    });
  });
});
