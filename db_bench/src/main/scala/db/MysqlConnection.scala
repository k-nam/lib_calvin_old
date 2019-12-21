package db

import java.sql.{ DriverManager, Connection };

class MysqlConnection(override val databaseName: String) extends DbConnection(databaseName) {
	val hostName = "localhost";
	//val hostName = "kocia.cytzyor3ndjk.ap-northeast-2.rds.amazonaws.com";
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
					"user=calvin&password=calvin1729&characterEncoding=UTF-8")
			println("mysql connection successful")
			conn.setAutoCommit(false);
			return conn

		} catch {
			case e: Throwable => { println("Connection error" + e.printStackTrace()); throw new Exception }
		}
	}
}