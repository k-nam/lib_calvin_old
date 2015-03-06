package lib.db.connection

import java.sql.{ DriverManager, Connection };
import scala.collection.mutable.ArrayBuffer

abstract class DbConnection(val databaseName: String) extends Thread {
	val connection: Connection

	override def run(): Unit = {
		pushData()
	}

	def pushData() {
		val tableName = "TEST"
		val insertStmt = connection.createStatement()
		try {
			insertStmt.executeUpdate("Truncate table " + tableName)
			println("Trucate OK");
		} catch {
			case _: Throwable => println("trunc fail");
		}
		Thread.sleep(1000)
		val stmt = connection.prepareStatement(
			"INSERT into " + tableName + " values (?, ?)")
		val longString = "남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅"
		val shortString = "남기웅남기웅남기웅남기웅남기웅남기웅"
		val wordString = "남기웅"
		val startTime: Double = System.currentTimeMillis()
		val testSize = 1000000
		val rowSize = 200
		for (i <- 1 to testSize) {
			stmt.setInt(1, i)
			stmt.setString(2, longString)
			stmt.addBatch()
			if (i % 1000 == 0) {
				stmt.executeBatch()
				connection.commit
			}
			if (i % 100000 == 0) {
				println("Now done " + i / 1000 + " k'th row")
			}
		}
		val endTime = System.currentTimeMillis()
		val timeConsumed = testSize / ((endTime - startTime) / 1000.0)
		println("TPS:\t" + timeConsumed / 1000.0 + " k / sec")
		//println("Throughput:\t" + timeConsumed * rowSize / 1000000 + " MB / sec")
	}
}

