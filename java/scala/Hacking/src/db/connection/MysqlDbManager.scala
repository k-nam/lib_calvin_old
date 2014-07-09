package db.connection

import java.sql.{ DriverManager, Connection };

class MysqlDbManage(override val databaseName: String) extends DbConnection(databaseName) {
	override val connection: Connection = getConnection()

	def getConnection(): Connection = {
		try {
			Class.forName("com.mysql.jdbc.Driver")
		} catch {
			case _ => {
				println("mysql driver not found")
				None
			}
		}
		try {
			val conn = DriverManager.getConnection(
				"jdbc:mysql://localhost:3306/" + databaseName + "?" +
					"user=root&password=calvin1729&characterEncoding=UTF-8")
			println("mysql connection successful")
			conn.setAutoCommit(false);
			return conn

		} catch {
			case _ => { println("Connection error"); throw new Exception }
		}
	}
}