package main
import lib.text.viewer._
import lib.text.copier._
import lib.db.connection.SqlServerConnection
import lib.db.loader._
import db.lookup._

object Main {
	def main(args: Array[String]): Unit = {
		//manageFile()
		hackingTest()
	}

	def manageFile(): Unit = {
		//new TextFileCopier("D:\\dev/data/hacking/data/utf8.txt", 
		//		"D:\\dev/data/hacking/data/password_under_20digit.txt").copyOnlyAscii(100000000, 20)	

	}

	def dataTest(): Unit = {
		//new Viewer().showLastLines("D:\\dev/data/hacking/data/password_under_20digit_first_last.txt", 100)
		//new Viewer().getNumLines("D:\\dev/data/hacking/data/password_under_20digit.txt")
		//new Viewer().getNumLines("D:\\dev/data/hacking/data/password_under_20digit_first_last.txt")
		new TextViewer().showLines("D:\\dev/data/hacking/data/password_under_20digit.txt", 10000, 100)
	}

	def hackingTest(): Unit = {
		//new TextFileLoader("D:\\dev/data/hacking/data/password_under_20digit.txt",
		//	new SqlServerConnection("Hacking").connection).loadPassword("Password.CommonPasswordUnder20Digit")

		//new TextFileLoader("D:\\dev/data/hacking/data/password_under_20digit_first_last.txt",
		//	new SqlServerConnection("Hacking").connection).loadFirstLast("RainbowTable.FirstLast_Md5_20Digit_SimpleReverse_10Iter")

		new ChainLookup("D:\\dev/data/hacking/test/chain.txt", "D:\\dev/data/hacking/test/first.txt",
			new SqlServerConnection("Hacking").connection, "RainbowTable.FirstLast_Md5_20Digit_SimpleReverse_10Iter")

	}
}