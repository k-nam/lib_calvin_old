"use strict"
const fs = require('fs');
const spawn = require('child_process').spawn;
const filename = process.argv[2];

if (filename === undefined) {
	throw Error("Filename must be specified");
}

fs.watch(filename, function() {
	let ls = spawn('dir', [filename]);
	//ls.stdout.pipe(process.stdout);
});
console.log('Now watching the file...');