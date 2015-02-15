package copier
import java.io._
import scala.io.Source
import java.nio.charset.Charset;

object TextFileCopier {
	// see if given UTF8 string is ASCII
	def isAsciiString(string: String): Boolean = {
		for (letter <- string) {
			if ((letter >=' ' && letter <= '~') || letter == '\t') {
			} else {
				//println(Integer.valueOf(letter))
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
		var previousLine = "null"
		var index = 0
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
					println("Last line was: " + previousLine)
					throw NullLine
				}
				previousLine = line
			} else {
				throw Break
			}
		}
	} catch {
		case Break => println("well done")
		case NullLine => println("well done, but whole file processed")
		case e: Exception => println("error: unknown")
	}
	writer.close()

}