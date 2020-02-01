package db

import java.sql.{ DriverManager, Connection };

class MysqlConnection(override val databaseName: String) extends DbConnection(databaseName) {
	val hostName = "localhost";
	// val hostName = "52.192.80.188";
	val password = "calvin1729";
	override val connection: Connection = getConnection()
	def getConnection(): Connection = {
		try {
			Class.forName("com.mysql.jdbc.Driver")
		} catch {
			case _: Throwable => {
				println("mysql driver not found")
				None
			}
		}
		try {
			println("Host name is: " + hostName)
			val conn = DriverManager.getConnection(
				"jdbc:mysql://" + hostName + ":3306/" + databaseName + "?" +
					"user=calvin&password=" + password + "&characterEncoding=UTF-8")
			println("mysql connection successful")
			conn.setAutoCommit(true);
			return conn

		} catch {
			case e: Throwable => { println("Connection error" + e.printStackTrace()); throw new Exception }
		}
	}
}