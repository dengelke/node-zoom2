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

#### Windows

Install yaz5 for your respective operating system via the FTP links for ([x86](https://ftp.indexdata.com/pub/yaz/win32/) or [x64](https://ftp.indexdata.com/pub/yaz/win64/))

### Installation

```bash
npm i node-zoom2
```

#### Common Installation Issues

If libyaz5-dev is not available on your distributions package manager by default run (or have a look at)

```bash
./preinstall.sh
```

Which will initalize the IndexData repository

## Example

```javascript
var zoom = require('node-zoom2');

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
