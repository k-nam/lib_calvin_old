package main
import lib.text.copier._

object Main {
	def main(args: Array[String]): Unit = {
		new TextFileCopier("D:\\5000_ansi.txt", "D:\\5000_ansi2.txt").swapTwoColumns()
	}
}