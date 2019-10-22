package db

import java.io._
import java.sql.{ Connection, PreparedStatement }
import scala.collection.mutable._;


class Loader(val sourceFileName: String, val connection: Connection, val tableName: String) {
	def loadPassword(): Unit = {
		new lib.db.loader.TextFileLoader(sourceFileName, connection, tableName).loadFile(
				1, (words, stmt) => stmt.setString(1, words(0)))
		println("loadPasswordList completed")
	}

	def loadFirstLast(): Unit = {
		new lib.db.loader.TextFileLoader(sourceFileName, connection, tableName).loadFile(
				2, (words, stmt) => ( stmt.setString(1, words(0)), stmt.setString(2, words(1))))
		println("loadFirstLast completed")
	}

}