package main
import lib.text.copier._
import lib.db.loader._
import lib.db.connection._
import java.sql.{ Connection, PreparedStatement }
import scala.collection.mutable._;

object Main {
	def main(args: Array[String]): Unit = {
		new TextFileCopier("D:\\5000_ansi.txt", "D:\\5000_ansi3.txt").swapTwoColumns()

	}

	def loadDictionary() {
		var set: Set[String] = new HashSet[String]()
			def op(words: Array[String], stmt: PreparedStatement) {
				if (words.length != 2) {
					print("error\n")
				}
				val word = words(0)
				if (set.contains(word)) {
					print("Duplicate: " + word + "\n")
				}
				set.add(word)
				stmt.setString(1, word)
				stmt.setInt(2, words(1).toInt)
			}
		new TextFileLoader("D:\\5000_ansi.txt", new SqlServerConnection("Web").connection, "Dictionary").
			loadFile(2, op)
		print("size was: " + set.size)
	}
}