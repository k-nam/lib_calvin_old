package main
import copier.TextFileCopier
import copier.BinaryFileCopier
import db.connection.SqlServerDbManager
import db.loader._
import db.lookup._

object Main {
	// first argument is the name of source file, 
	// second is the name of target file, 
	// third is number of lines to copy 
	def main(args: Array[String]): Unit = {
		//new TextFileCopier("E:\\UTF8.txt", "E:\\ascii.txt", 300000000, 20)
		//new BinaryFileCopier("realhuman_phill.txt", "output.txt", 1000000)

		//new SqlServerDbManager("aaa").start
		//		new TextFileLoader("D:\\hacking/data/password_under_20digit.txt",
		//			new SqlServerDbManager("Hacking").connection).loadPassword("Password.CommonPasswordUnder20Digit")
		//		new TextFileLoader("D:\\hacking/data/password_under_20digit_first_last.txt",
		//			new SqlServerDbManager("Hacking").connection).loadFirstLast("RainbowTable.FirstLast_Md5_20Digit_SimpleReverse_10Iter")

		new ChainLookup("D:\\hacking/test/chain.txt", "D:\\hacking/test/first.txt",
			new SqlServerDbManager("Hacking").connection).lookUpChain("RainbowTable.FirstLast_Md5_20Digit_SimpleReverse_10Iter")
	}
}