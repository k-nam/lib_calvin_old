package lib.text.copier
import java.io._
import scala.io.Source
import java.nio.charset.Charset;

class TextFileCopier(val sourceFileName: String, val targetFileName: String) {
	object Break extends Exception
	object NullLine extends Exception

	// see if given UTF8 string is ASCII
	def isAsciiString(string: String): Boolean = {
		for (letter <- string) {
			if ((letter >= ' ' && letter <= '~') || letter == '\t') {
			} else {
				//println(Integer.valueOf(letter))
				return false
			}
		}
		return true
	}

	def copyOnlyAscii(numLines: Int, maxLineLength: Int) {
		var remainingNumLines = numLines
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(sourceFileName), "UTF8"))
		val writer = new PrintWriter(new File(targetFileName))
		try {
			var previousLine = "null"
			var index = 0
			while (true) {
				if (remainingNumLines > 0) {
					val line = reader.readLine()
					if (line != null) {
						if (line.length <= maxLineLength && isAsciiString(line) && line.length() != 0) {
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
			case Break => println("copyOnlyAscii well done, part of file processed")
			case NullLine => println("copyOnlyAscii well done, whole file processed")
			case e: Exception => println("error: unknown" + e.printStackTrace())
		} finally {
			reader.close()
			writer.close()
		}
	}

	def copyWithOperation(operation: Array[String] => Array[String]) {
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(sourceFileName), "UTF8"))
		val writer = new PrintWriter(new File(targetFileName))
		try {

			while (true) {
				val line = reader.readLine()
				if (line != null) {
					val words = line.split('\t')
					val wordsAfterOp = operation(words)
					val lineAfterOp = wordsAfterOp.mkString("\t") 
					writer.write(lineAfterOp)
					writer.write("\n")
				} else {
					throw NullLine
				}
			}
		} catch {
			case NullLine => println("copyWithOperation well done")
			case e: Exception => println("error: unknown" + e.printStackTrace())
		} finally {
			reader.close()
			writer.close()
		}
	}
	
	def swapTwoColumns() {
		def swap(words: Array[String]): Array[String] = {
			Array[String](words(1), words(0))
		}
		copyWithOperation(swap)
	}
}