package db.loader
import copier._
import java.io._
import java.sql.{ DriverManager, Connection };

class TextFileLoader(val sourceFileName: String, val connection: Connection) {
	def loadPasswordList(tableName: String): Unit = {
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(sourceFileName), "UTF8"))
		val insertStmt = connection.createStatement
		val stmt = connection.prepareStatement(
			"INSERT INTO " + tableName + " values (?)")
		connection.setAutoCommit(false)

		var thisLine: String = reader.readLine
		var i = 0
		while (thisLine != null) {
			println(thisLine)
			if (TextFileCopier.isAsciiString(thisLine) == false) {
				println("TextFileLoader: string is not Ascii");
				return
			}
			stmt.setString(1, thisLine)
			stmt.addBatch
			thisLine = reader.readLine
			i += 1
			if (i % 1000 == 0) {
				stmt.executeBatch()
				connection.commit
			}
		}
		stmt.executeBatch()
		connection.commit
	}
	def loadChainList(tableName: String): Unit = {
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(sourceFileName), "UTF8"))
		val insertStmt = connection.createStatement
		val stmt = connection.prepareStatement(
			"INSERT INTO " + tableName + " values (?, ?)")
		connection.setAutoCommit(false)

		var thisLine: String = reader.readLine
		var i = 0
		var turn = 0; // 0: time to read start password, 1: time to read second, 2: time to read empty line
		while (thisLine != null) {
			println(thisLine)
			if (TextFileCopier.isAsciiString(thisLine) == false) {
				println("TextFileLoader: string is not Ascii");
				return
			}
			if (turn == 0) {
				stmt.setString(1, thisLine)
			} else if (turn == 1) {
				stmt.setString(2, thisLine)
			} else {
				if (thisLine != "") {
					println("loadChainList error")
					return
				}
				stmt.addBatch
			}
			thisLine = reader.readLine
			turn += 1
			turn %= 3
			i += 1
			if (i % 1000 == 0) {
				stmt.executeBatch()
				connection.commit
			}
		}
		if (turn != 2) {
			println("loadChainList error2")
			return
		}
		if (i % 1000 != 0) {
			stmt.addBatch
			stmt.executeBatch()
			connection.commit
		}
	}
}