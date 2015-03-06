package lib.text.copier
import java.io._
import scala.util._
import scala.util.control.Breaks._
import scala.io.Source
import java.nio.charset.Charset;
import scala.collection.mutable._;

class TextFileCopier(val sourceFileName: String, val targetFileName: String) {
	object Break extends Exception
	object NullLine extends Exception
	object Impossible extends Exception

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

	// process each line

	def copyWithLineOperation(operation: String => String) {
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(sourceFileName), "UTF8"))
		val writer = new PrintWriter(new File(targetFileName))
		try {
			while (true) {
				val line = reader.readLine()
				if (line != null) {
					val lineAfterOp = operation(line)
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

	// process whole file as a chunk 
	def copyWithOperation(operation: Array[String] => Array[String]) {
			def readTextFileToStrings(sourceFileName: String): Array[String] = {
				val reader = new BufferedReader(new InputStreamReader(new FileInputStream(sourceFileName), "UTF8"))
				val lines = ArrayBuffer[String]()
				try {
					while (true) {
						val line = reader.readLine()
						if (line != null) {
							lines.append(line)
						} else {
							throw NullLine
						}
					}
				} catch {
					case NullLine => println("readTextFileToStrings well done")
				}
				reader.close()
				lines.toArray
			}

			def writeStringsToTextFile(lines: Array[String], targetFileName: String) {
				val writer = new PrintWriter(new File(targetFileName))
				for (line <- lines) {
					writer.write(line)
					writer.write("\n")
				}
				writer.close()
			}
		val lines = readTextFileToStrings(sourceFileName)
		writeStringsToTextFile(operation(lines), targetFileName)
	}

	def swapTwoColumns() {
			def swap(line: String): String = {
				val words = line.split("\t")
				Array[String](words(1), words(0)).mkString("\t")
			}
		copyWithLineOperation(swap)
	}
}