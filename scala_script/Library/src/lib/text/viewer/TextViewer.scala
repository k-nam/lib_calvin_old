package lib.text.viewer
import java.io._

class TextViewer {
	def getNumLines(fileName: String): Unit = {
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(fileName), "UTF8"))
		var numLines = 0
		while (reader.readLine != null) {
			numLines += 1
		}
		println("File: " + fileName + " numLines: = " + numLines)
	}

	def showLines(fileName: String, startLine: Int, numLines: Int): Unit = {
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(fileName), "UTF8"))
		var index = 0
		if (startLine >= 0 && numLines > 0) {

		} else {
			println("showLines argument error")
			return
		}
		object End extends Exception
		try {
			while (true) {
				val thisLine = reader.readLine
				if (thisLine == null) {
					throw End
				}
				if (index == startLine + numLines) {
					throw End
				} else if (index >= startLine) {
					println("\"" + thisLine + "\"")
				}
				index += 1
			}
		} catch {
			case End => println("------------ showLines End of result -------------------")
		}

	}

	// show last num lines of a text file
	def showLastLines(fileName: String, numLines: Int): Unit = {
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(fileName), "UTF8"))
		val lastLines = new Array[String](numLines);
		// we use 0 <= x < lastIndexInArray when not full,
		// we use lastIndexInArray <= x < lastIndexInArray when full
		var lastIndexInArray = 0
		var isArrayFull = false
		object NullLine extends Exception
		try {
			while (true) {
				val thisLine = reader.readLine
				if (thisLine != null) {
					lastLines(lastIndexInArray) = thisLine
					lastIndexInArray += 1
					if (lastIndexInArray == numLines) {
						isArrayFull = true
						lastIndexInArray = 0
					}
				} else { // end of file
					throw NullLine
				}
			}
		} catch {
			case NullLine => println("showLastLines end of file")
		}
		for (index <- 0 to numLines - 1) {
			val line = "\"" + lastLines((lastIndexInArray + index) % numLines) + "\""
			println(line)
		}
		println("------------ showLastLines End of result -------------------")
	}
}