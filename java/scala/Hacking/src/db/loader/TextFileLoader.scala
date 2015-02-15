package db.loader
import copier._
import java.io._
import java.sql.{ DriverManager, Connection, PreparedStatement };
import scala.collection.mutable._;

class TextFileLoader(val sourceFileName: String, val connection: Connection) {
	def loadPassword(tableName: String): Unit = {
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(sourceFileName), "UTF8"))
		val batchStmt = connection.prepareStatement("INSERT INTO " + tableName + " values (?)")
		val batchSize = 10000
		connection.setAutoCommit(false)
		val queries = new HashSet[PreparedStatement]()
		var password: String = reader.readLine
		var i = 0
			def createStmt(password: String): PreparedStatement = {
				val stmt = connection.prepareStatement(
					"INSERT INTO " + tableName + " values (?)")
				stmt.setString(1, password)
				stmt
			}

		while (password != null) {
			batchStmt.setString(1, password)
			batchStmt.addBatch
			queries.add(createStmt(password))
			i += 1
			if (i % batchSize == 0) {
				println("Executing batch, current line is " + password)
				executeBatch(batchStmt, queries)
			}
			password = reader.readLine
		}
		if (i % batchSize != 0) {
			executeBatch(batchStmt, queries)
		}
		print("loadPasswordList completed")
	}

	def loadFirstLast(tableName: String): Unit = {
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(sourceFileName), "UTF8"))
		val batchStmt = connection.prepareStatement(
			"INSERT INTO " + tableName + " values (?, ?)")
		var first: String = null
		connection.setAutoCommit(false)
			def createStmt(first: String, last: String): PreparedStatement = {
				val stmt = connection.prepareStatement(
					"INSERT INTO " + tableName + " values (?, ?)")
				stmt.setString(1, first)
				stmt.setString(2, last)
				stmt
			}
		val batchSize = 10000
		val queries = new HashSet[PreparedStatement]()
		var thisLine: String = reader.readLine
		var lineIndex = 0
		var i = 0
		var turn = 0; // 0: time to read start password, 1: time to read second, 2: time to read empty line
		while (thisLine != null) {
			if (turn != 2 && thisLine.equals("")) {
				println("error1 empty line in #line: " + lineIndex)
				return
			}
			if (turn == 0) {
				batchStmt.setString(1, thisLine)
				first = thisLine
			} else if (turn == 1) {
				batchStmt.setString(2, thisLine)
				batchStmt.addBatch
				queries.add(createStmt(first, thisLine))
				i += 1
				if (i % batchSize == 0) {
					println("Executing batch, current line is " + thisLine + " turn was " + turn)
					executeBatch(batchStmt, queries)
				}
			} else {
				if (!thisLine.equals("")) {
					println("error2 empty line in #line: " + lineIndex)
					return
				}
			}
			turn += 1
			turn %= 3
			thisLine = reader.readLine
			lineIndex += 1
		}
		if (turn != 2) {
			println("error3 turn error in #line: " + lineIndex)
			return
		}
		if (i % batchSize != 0) {
			executeBatch(batchStmt, queries)
		}
		println("loadFirstLast completed")
	}

	def executeBatch(batchStmt: PreparedStatement, querySet: Set[PreparedStatement]): Unit = {
		executeBatchOrQueries(batchStmt, querySet)
		querySet.clear
		connection.commit
			// do batch, or do each queries if fails
			def executeBatchOrQueries(batch: PreparedStatement, querySet: Set[PreparedStatement]): Unit = {
				try {
					batch.executeBatch
				} catch {
					case e: Exception => {
						//println(e)
						for (stmt <- querySet) {
							try {
								stmt.execute
							} catch {
								case e: Exception => println(e)
							}
						}
					}
				}
			}
	}

}