package copier
import java.io._
import scala.io.Source
import java.nio.charset.Charset;

class TextFileCopier(val sourceFileName: String, val targetFileName: String, val numLines: Int) {
	val writer = new PrintWriter(new File(targetFileName))
	var remainingNumLines = numLines
	object Break extends Exception

	try {
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(sourceFileName), "UTF8"))
		while (true) {
			if (remainingNumLines > 0) {
				val line = reader.readLine()
				if (line != null) {
					if (remainingNumLines < numLines) { // not first line
						writer.write('\n')
					}
					writer.write(line)
					remainingNumLines -= 1
				} else {
					throw Break
				}
			} else {
				throw Break
			}
		}
	} catch {
		case Break => print("well done")
		case e: Exception => print("error")
	}

	writer.close()
}