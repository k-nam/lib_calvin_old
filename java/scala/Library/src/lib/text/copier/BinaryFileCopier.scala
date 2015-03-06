package lib.text.copier
import java.io._
import scala.io.Source

// For detecting Encoding. 
// Encoding is delicate matter. It showed different results for different substring of the same document.
class BinaryFileCopier(val sourceFileName: String, val targetFileName: String, val numBytes: Int) {
	val writer = new BufferedOutputStream(new FileOutputStream(targetFileName))
	var remainingNumBytes = numBytes
	try {
		val bis = new BufferedInputStream(new FileInputStream(sourceFileName))
		var buffer: Array[Byte] = new Array[Byte](numBytes)
		bis.read(buffer, 0, numBytes)
		println("Encoding was: " + guessEncoding(buffer))
		writer.write(buffer, 0, numBytes)
	} catch {
		case e: Exception => println("error")
	} finally {
		println("well done")
	}

	writer.close()

	def guessEncoding(bytes: Array[Byte]): String = {
		val DEFAULT_ENCODING = "UTF-8";
		val detector = new org.mozilla.universalchardet.UniversalDetector(null);
		detector.handleData(bytes, 0, bytes.length);
		detector.dataEnd();
		val encoding = detector.getDetectedCharset();
		detector.reset();
		if (encoding == null) {
			"Unknown encoding"
		}
		encoding;
	}

}