package lib.db.loader

import java.io._
import java.sql.{ Connection, PreparedStatement }
import scala.collection.mutable._;

class TextFileLoader(val sourceFileName: String, val connection: Connection, val tableName: String) {
	// Generic data load function
	def loadFile(numColumns: Int, operation: (Array[String], PreparedStatement) => Unit) {
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
								println("Error in batch:" + e)
								for (stmt <- querySet) {
									try {
										stmt.execute
									} catch {
										case e: Exception => println("Error in query:" + e)
									}
								}
							}
						}
					}
			}
		val reader = new BufferedReader(new InputStreamReader(new FileInputStream(sourceFileName), "UTF8"))
			def getPreparedStmt: PreparedStatement = {
				var columnVariableString = "(?"
				for (i <- 1 until numColumns) {
					columnVariableString += ", ?"
				}
				columnVariableString += ")"
				val preparedStmt = connection.prepareStatement("INSERT INTO " + tableName + " values " + columnVariableString)
				preparedStmt
			}

		val batchStmt = getPreparedStmt
		connection.setAutoCommit(false)
		val batchSize = 1000
		val queries = HashSet[PreparedStatement]()
		var thisLine = reader.readLine
		var i = 0
		while (thisLine != null) {
			if (thisLine.equals("")) { // fine, next in line please.
			} else {
				val words = thisLine.split("\t")
				operation(words, batchStmt)
				batchStmt.addBatch
				val stmtForThisLine = getPreparedStmt
				operation(words, stmtForThisLine)
				queries.add(stmtForThisLine)
				i += 1
				if (i % batchSize == 0) {
					println("Executing batch, current line is " + thisLine)
					executeBatch(batchStmt, queries)
				}
			}
			thisLine = reader.readLine
		}
		if (i % batchSize != 0) {
			executeBatch(batchStmt, queries)
		}
		println("loadData completed")
	}
}
