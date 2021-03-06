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
  if (argv.length === 3) {
    var fs = require('fs');
    var filename = argv[2];
    var pem = fs.readFileSync(filename, 'utf8').trim();
    var convertedPem = convcert(pem);
    fs.writeFileSync('./certificate.h', convertedPem);
  }
} catch (e) {
  // browser
}
