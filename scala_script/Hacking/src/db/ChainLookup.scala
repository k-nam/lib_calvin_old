package db
import java.sql._
import java.io._
import scala.collection.mutable._;

class ChainLookup(val chainFileName: String, val outputFileName: String,
		val connection: Connection, val tableName: String) {

	def getSelectQuery(last: String): String = {
		"SELECT first FROM " + tableName + " WHERE last = " + "\'" + last + "\'"
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
		if (line == "") { // fine just jump
		} else {
			val words = line.split('\t');
			val hash = words(0)
			writer.write(hash + "\t") // copy hash
			val passwords = words.slice(1, words.length)
			for (password <- passwords) {
				val query = getSelectQuery(password)
				//println(query)
				val rs = stmt.executeQuery(query)
				while (rs.next) {
					writer.write(rs.getString("first"))
					writer.write("\t")
				}
			}
			writer.write("\n")
		}
		line = reader.readLine
	}
	writer.close
	println("lookup chain completed")

}