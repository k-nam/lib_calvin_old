package db

import java.sql.{ DriverManager, Connection }; ;

class SqlServerDbManager extends DbManager {
	val connection: Connection = getConnection()

	def getConnection(): Connection = {
		try {
			Class.forName("com.microsoft.sqlserver.jdbc.SQLServerDriver")
		} catch {
			case _ => {
				println("sqlserver driver not found")
				None
			}
		}
		try {
			val conn = DriverManager.getConnection(
				"jdbc:sqlserver://192.168.0.15\\CALVIN_MSSQL:1433;databaseName=aaa",
				"sa", "calvin1729")
			conn.setAutoCommit(false)
			println("SQL server connection successful")
			return conn
		} catch {
			case e: Throwable => { println("Connection error"); println(e); throw new Exception }
		}

	}
}