package main
import copier.TextFileCopier
import copier.BinaryFileCopier
import db.connection.SqlServerDbManager
import db.loader._
import db.lookup._
import TextViewer._

object Main {
	// first argument is the name of source file, 
	// second is the name of target file, 
	// third is number of lines to copy 
	def main(args: Array[String]): Unit = {
		hackingTest()
	}

	def manageFile(): Unit = {
		//new TextFileCopier("D:\\dev_data/hacking/data/utf8.txt", 
		//		"D:\\dev_data/hacking/data/password_under_20digit.txt", 100000000, 20)		
	}
	
	def dataTest(): Unit = {
		//new Viewer().showLastLines("D:\\dev_data/hacking/data/password_under_20digit_first_last.txt", 100)
		//new Viewer().getNumLines("D:\\dev_data/hacking/data/password_under_20digit.txt")
		//new Viewer().getNumLines("D:\\dev_data/hacking/data/password_under_20digit_first_last.txt")
		new Viewer().showLines("D:\\dev_data/hacking/data/password_under_20digit.txt", 10000, 100)
	}

	def hackingTest(): Unit = {
		//new TextFileLoader("D:\\dev_data/hacking/data/password_under_20digit.txt",
		//	new SqlServerDbManager("Hacking").connection).loadPassword("Password.CommonPasswordUnder20Digit")

		//new TextFileLoader("D:\\dev_data/hacking/data/password_under_20digit_first_last.txt",
			//new SqlServerDbManager("Hacking").connection).loadFirstLast("RainbowTable.FirstLast_Md5_20Digit_SimpleReverse_10Iter")

		new ChainLookup("D:\\dev_data/hacking/test/chain.txt", "D:\\dev_data/hacking/test/first.txt",
			new SqlServerDbManager("Hacking").connection).lookUpChain("RainbowTable.FirstLast_Md5_20Digit_SimpleReverse_10Iter")

	}
}