package main
import lib.text.copier._
import lib.db.loader._
import lib.db.connection._
import java.sql.{ Connection, PreparedStatement }
import scala.collection.mutable._;

object Main {
	def main(args: Array[String]): Unit = {
		//new TextFileCopier("D:\\5000_ansi.txt", "D:\\5000_ansi3.txt").swapTwoColumns()
		loadDictionary()

	}

	def loadDictionary() {
		var set: Set[String] = new HashSet[String]()
			def op(words: Array[String], stmt: PreparedStatement) {
				val word = words(0)
				if (set.contains(word)) {
					//println("Duplicate: " + word)
				}
				set.add(word)
				stmt.setString(1, word)
				//println("current word: " + word)
				if (words.length == 1) { // 100k dict without rank field
					stmt.setNull(2, java.sql.Types.INTEGER)
				} else if (words.length == 2) { // 5k dict witih rank field
					stmt.setInt(2, words(1).toInt)
				} else {
					println("loadDictionary error")
					System.exit(0)
				}
			}

			def loadToSqlserver() {
				new TextFileLoader("D:\\dev/data/wordlist/Eng_5K.txt",
					new SqlServerConnection("Web").connection, "Wordlist.Eng_100K").loadFile(2, op)
				new TextFileLoader("D:\\dev/data/wordlist/Eng_100K.txt",
					new SqlServerConnection("Web").connection, "Wordlist.Eng_100K").loadFile(2, op)
			}

			def loadToMysql() {
				new TextFileLoader("D:\\dev/data/wordlist/Eng_5K.txt",
					new MysqlConnection("web").connection, "eng_100k").loadFile(2, op)
				new TextFileLoader("D:\\dev/data/wordlist/Eng_100K.txt",
					new MysqlConnection("web").connection, "eng_100k").loadFile(2, op)
			}
		//loadToSqlserver()
		loadToMysql()
		println("size was: " + set.size)
	}
}