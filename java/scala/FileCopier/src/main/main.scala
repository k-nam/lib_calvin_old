package main
import copier.TextFileCopier
import copier.BinaryFileCopier

object Main {
	// first argument is the name of source file, 
	// second is the name of target file, 
	// third is number of lines to copy 
	def main(args: Array[String]): Unit = {
		new TextFileCopier("utf8.txt", "output.txt", 100000000)
		//new BinaryFileCopier("realhuman_phill.txt", "output.txt", 1000000)
	}

}