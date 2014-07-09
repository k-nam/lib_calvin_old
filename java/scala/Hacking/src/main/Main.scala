package main
import copier.TextFileCopier
import copier.BinaryFileCopier
import db.connection.SqlServerDbManager
import db.loader.TextFileLoader

object Main {
	// first argument is the name of source file, 
	// second is the name of target file, 
	// third is number of lines to copy 
	def main(args: Array[String]): Unit = {
		//new TextFileCopier("E:\\UTF8.txt", "E:\\ascii.txt", 300000000, 20)
		//new BinaryFileCopier("realhuman_phill.txt", "output.txt", 1000000)

		//new SqlServerDbManager("aaa").start
		new TextFileLoader("E:\\hacking/password_under_20digit.txt", new SqlServerDbManager("Hacking").connection).
			loadPasswordList("Password.CommonPasswordUnder20Digit")
		//new TextFileLoader("E:\\hacking/chain.txt", new SqlServerDbManager("Hacking").connection).
		//loadChainList("RainbowTableChain.Md5_20Digit_SimpleReduce_10Iter")
	}
}