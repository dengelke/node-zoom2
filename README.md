NODE-ZOOM2
=========

[![npm package](https://nodei.co/npm/node-zoom2.png?downloads=true&downloadRank=true&stars=true)](https://nodei.co/npm/node-zoom2/)

[![Build Status](https://travis-ci.org/dengelke/node-zoom2.svg?branch=master)](https://travis-ci.org/dengelke/node-zoom2)
[![Coverage Status](https://coveralls.io/repos/github/dengelke/node-zoom2/badge.svg?branch=master)](https://coveralls.io/github/dengelke/node-zoom2?branch=master)

### Requirements

* [YAZ](http://www.indexdata.dk/yaz/)

#### Debian/Ubuntu

```bash
sudo apt-get install -y libyaz5-dev
```

#### Mac

```bash
brew install yaz
```

### Installation

```bash
npm i node-zoom2
```

## Example

```javascript
var zoom = require('node-zoom2');

zoom.connection('192.83.186.170:210/INNOPAC')
  .set('preferredRecordSyntax', 'usmarc')
  .query('prefix', '@attr 1=4 台灣')
  .createReadStream()
  .on('data', function (record) {
    console.log(record.json);
  });
```

## API

### Connection

* `#set(optName, optValue)`
* `#get(optName)`
* `#query([type], querystring)`
* `#search(callback)`
* `#createReadStream([options])`

### ResultSet

* `.size`
* `#getRecords(start, count, callback)`

### Records

* `#hasNext()`
* `#next()`

### Record

* `.json`
* `.database`
* `.syntax`
* `.schema`
* `.render`
* `.raw`
* `.xml`
* `.txml`