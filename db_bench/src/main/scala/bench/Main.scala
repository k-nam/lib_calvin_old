package bench
import db._

object Main {
	def main(args: Array[String]): Unit = {
		dbBench()
	}

	def dbBench(): Unit = {
		val numThead = 15
		for (i <- 1 to numThead) {
			new MysqlConnection("movie").start()

		}

	}
}