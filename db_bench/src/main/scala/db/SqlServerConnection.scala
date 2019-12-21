package db

import java.sql.{ DriverManager, Connection }

class SqlServerConnection(override val databaseName: String) extends DbConnection(databaseName) {
	override val connection: Connection = {
		try {
			Class.forName("com.microsoft.sqlserver.jdbc.SQLServerDriver")
		} catch {
			case e: Throwable => {
				println("sqlserver driver not found")
				println(e)
				throw new Exception
			}
		}
		try {
			val conn = DriverManager.getConnection(
				"jdbc:sqlserver://localhost:1433;databaseName=" + databaseName,
				"calvin", "password")
			conn.setAutoCommit(false)
			println("SQL server connection successful")
			conn
		} catch {
			case e: Throwable => { println("Connection error"); println(e); throw new Exception }
		}
	}
}