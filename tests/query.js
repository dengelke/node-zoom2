'use strict';

var expect = require('chai').expect;
var Query = require('..').binding.Query;

describe('Query', function () {

  describe('constructor()', function () {
    it('should work', function () {
      new Query();
      Query();
    });
  });

  describe('#prefix()', function () {
    it('should work', function () {
      Query().prefix('@attr 1=4 台灣');
    });
  });

  describe('#cql()', function () {
    it('should work', function () {
      Query().cql('dc.title any fish or/rel.combine=sum dc.creator any sanderson');
      Query().cql('dc.title any fish prox/unit=word/distance>3 dc.title any squirrel');
    });
  });

});
