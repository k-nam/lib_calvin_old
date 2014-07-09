package copier
import java.io._
import scala.io.Source
import java.nio.charset.Charset;

object TextFileCopier {
	// see if given UTF8 string is ASCII
	def isAsciiString(string: String): Boolean = {
		for (letter <- string) {
			if (letter < ' ' || letter > '~') {
				return false
			}
		}
		return true
	}

}

class TextFileCopier(val sourceFileName: String, val targetFileName: String, val numLines: Int, val maxLineLength: Int) {
	val writer = new PrintWriter(new File(targetFileName))
	var remainingNumLines = numLines
	object Break extends Exception
	object NullLine extends Exception

	try {
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(sourceFileName), "UTF8"))
		while (true) {
			if (remainingNumLines > 0) {
				val line = reader.readLine()
				if (line != null) {
					if (line.length <= maxLineLength && TextFileCopier.isAsciiString(line) && line.length() != 0) {
						if (remainingNumLines < numLines) { // not first line
							writer.write('\n')
						}
						writer.write(line)
						remainingNumLines -= 1
					} else {
					}
				} else {
					throw NullLine
				}
			} else {
				throw Break
			}
		}
	} catch {
		case Break => print("well done")
		case NullLine => print("well done, but whole file processed")
		case e: Exception => print("error: unknown")
	}
	writer.close()

}