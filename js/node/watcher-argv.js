const fs = require('fs');
const filename = process.argv[2];

if (filename === undefined) {
	throw Error("Filename must be specified");
}

fs.watch(filename, function() {
	console.log("File 'target.txt' has changed.");
});
console.log('Now watching the file...');