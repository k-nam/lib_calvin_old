package lib.db.connection

import java.sql.DriverManager;

class OracleConnection(override val databaseName: String) extends DbConnection(databaseName) {
	val connection: oracle.jdbc.OracleConnection = getConnection()

	def getConnection(): oracle.jdbc.OracleConnection = {
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
				"jdbc:oracle:thin:id/password@//localhost:1521/orcl")
			println("Oracle connection successful")
			//val stmt = conn.prepareStatement("ALTER SESSION set TIME_ZONE = '00:00'")
			//stmt.executeQuery()
			//println("set session time zone")
			conn.setAutoCommit(true);
			return conn.asInstanceOf[oracle.jdbc.OracleConnection]
		} catch {
			case e: Exception => { println("Oracle Connection error"); println(e); throw new Exception }
		}
	}

}

