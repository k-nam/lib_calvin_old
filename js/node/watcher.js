const fs = require('fs');
fs.watch('target.txt', function() {
	console.log("File 'target.txt' has changed.");
});
console.log('Now watching the file...');