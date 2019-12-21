package db

import java.sql.{ DriverManager, Connection };


// jdbc:postgresql:database
// jdbc:postgresql:/
// jdbc:postgresql://host/database
// jdbc:postgresql://host/
// jdbc:postgresql://host:port/database
// jdbc:postgresql://host:port/

// class MysqlConnection(override val databaseName: String) extends DbConnection(databaseName) {
// 	val hostName = "localhost";
// 	override val connection: Connection = getConnection()
// 	def getConnection(): Connection = {
// 		try {
// 			Class.forName("org.postgresql.Driver")
// 		} catch {
// 			case _: Throwable => {
// 				println("postgresql driver not found")
// 				None
// 			}
// 		}
// 		try {
// 			println("Host name is: " + hostName)
// 			val conn = DriverManager.getConnection(
// 				"jdbc:postgresql://" + hostName + ":5432/" + databaseName + "?" +
// 					"user=calvin&password=calvin1729&ssl=true")
// 			println("postgresql connection successful")
// 			conn.setAutoCommit(false);
// 			return conn

// 		} catch {
// 			case e: Throwable => { println("Connection error" + e.printStackTrace()); throw new Exception }
// 		}
// 	}
// }