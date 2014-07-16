package db.lookup
import java.sql._
import java.io._
import scala.collection.mutable._;

class ChainLookup(val chainFileName: String, val outputFileName: String, val connection: Connection) {
	def lookUpChain(tableName: String) = {
			def getSelectQuery(last: String): String = {
				"SELECT First FROM " + tableName + " WHERE Last = " + "\'" + last + "\'"
			}
		connection.setAutoCommit(false)
		val stmt = connection.createStatement
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(chainFileName), "UTF8"))
		val writer = new PrintWriter(new File(outputFileName))
		var line: String = null
		var turn: Int = 0; // 0: read hash, 1: read chain
		var isFirstLine = true
		line = reader.readLine
		while (line != null) {
			if (turn == 0) {
				if (isFirstLine) {
					isFirstLine = false
				} else {
					writer.write("\n\n")
				}
				//println("hash is: " + line)
				writer.write(line) // copy hash
				turn = 1;
			} else {
				if (line.equals("")) { // end of this chain
					turn = 0;
				} else {
					val query = getSelectQuery(line)
					//println(query)
					val rs = stmt.executeQuery(query)

					while (rs.next) {
						writer.write("\n")
						writer.write(rs.getString("First"))

					}
				}
			}
			line = reader.readLine
		}
		writer.close
		println("lookup chain completed")
	}
}