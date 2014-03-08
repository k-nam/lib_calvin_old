package db

import java.sql.DriverManager;
import oracle.jdbc.OracleConnection

class OracleDbManager extends DbManager {
	val connection: OracleConnection = getConnection()

	def getConnection(): OracleConnection = {
		try {
			Class.forName("oracle.jdbc.OracleDriver")
		} catch {
			case _ => {
				println("oracle jdbc not found")
				None
			}
		}
		try {
			val conn = DriverManager.getConnection(
				"jdbc:oracle:thin:calvin/calvin1729@//localhost:1521/orcl")
			println("Oracle connection successful")
			//val stmt = conn.prepareStatement("ALTER SESSION set TIME_ZONE = '00:00'")
			//stmt.executeQuery()
			//println("set session time zone")
			conn.setAutoCommit(true);
			return conn.asInstanceOf[OracleConnection]
		} catch {
			case e: Exception => { println("Oracle Connection error"); println(e); throw new Exception }
		}
	}

}

