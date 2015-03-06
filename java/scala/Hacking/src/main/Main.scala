package main
import db._
import lib.text.viewer._
import lib.text.copier._
import lib.db.connection.SqlServerConnection

object Main {
	def main(args: Array[String]): Unit = {
		//manageFile()
			hackingTest()
		//hackingReal()
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
		//new Loader("D:\\dev/data/hacking/data/10K.txt",
		//	new SqlServerConnection("Hacking").connection, "Password.[10K]").loadPassword()

		new Loader("D:\\dev/data/hacking/data/FirstLast_10K.txt",
			new SqlServerConnection("Hacking").connection, "RainbowTable.[FirstLast_10K]").loadFirstLast

		//	new ChainLookup("D:\\dev/data/hacking/test/chain.txt", "D:\\dev/data/hacking/test/first.txt",
		//new SqlServerConnection("Hacking").connection, "RainbowTable.FirstLast_Md5_20Digit_SimpleReverse_10Iter")

	}

	def hackingReal() {
		//new Loader("D:\\dev/data/hacking/data/61M.txt",
		//	new SqlServerConnection("Hacking").connection, "Password.[61M]").loadPassword()

		new Loader("D:\\dev/data/hacking/data/FirstLast_61M.txt",
			new SqlServerConnection("Hacking").connection, "RainbowTable.[FirstLast_61M]").loadFirstLast

		//	new ChainLookup("D:\\dev/data/hacking/test/chain.txt", "D:\\dev/data/hacking/test/first.txt",
		//new SqlServerConnection("Hacking").connection, "RainbowTable.FirstLast_Md5_20Digit_SimpleReverse_10Iter")
	}
}