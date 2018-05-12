'use strict';

function convcert(pem) {
  var lines = [];
  lines.push('const char* test_ca_cert = \\');
  pem.split('\n').forEach(function (str) {
    lines.push('"' + str + '\\n" \\');
  });
  lines.push(';\n');

  return lines.join('\n');
}

try {
  var argv = process.argv;
  if (process.argv.length === 3) {
    var fs = require('fs');
    var filename = process.argv[2];
    var pem = fs.readFileSync(filename, 'utf8').trim(); // String.prototype.trim(): ES2015
    var convertedPem = convcert(pem);
    fs.writeFileSync('./certificate.h', convertedPem);
  }
} catch (e) {
  // browser
}
