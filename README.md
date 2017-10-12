NODE-ZOOM2
=========

### Requirements

* [gnutls](http://www.gnutls.org/)
* [gcrypt](http://www.gnu.org/software/libgcrypt/)
* [libxml2](http://xmlsoft.org/)

#### Debian/Ubuntu

```bash
$ sudo apt-get install libgcrypt11-dev libgnutls28-dev libxml2-dev
```

### Installaction

```bash
$ npm i node-zoom2
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

## License

The MIT License

Copyright © 2017

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
