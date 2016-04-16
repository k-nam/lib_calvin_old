package db

import java.sql.{ DriverManager, Connection };
import scala.collection.mutable.ArrayBuffer

abstract class DbManager extends Thread {
	val connection: Connection

	override def run(): Unit = {
		pushData()
		connection.close()
	}

	def getPersonList(): Set[String] = {
		val stmt = connection.createStatement()
		val rs = stmt.executeQuery("SELECT id from Me2Person")
		var people = Set[String]()
		while (rs.next()) {
			val id = rs.getString("id")
			people += id
		}
		stmt.close()
		people
	}

	def getNewPeple(): List[String] = {
		val people: Set[String] = getPersonList()
		var friends = Set[String]()
		for (person <- people) {
			val friendsList: List[String] = {
				getPerson(person) match {
					case Some(person) => person.friendsList
					case None => List[String]()
				}
			}
			for (friend <- friendsList) {
				friends += friend
			}
		}
		println("a")
		for (person <- people) {
			friends -= person
		}
		println("b")
		var result = List[String]()
		for (friend <- friends) {
			result = friend :: result
		}
		result
	}

	def updatePerson(person: Me2Person): Unit = {
		getPerson(person.id) match {
			case Some(found) => return
			case None =>
				val stmt = connection.prepareStatement("INSERT into ME2PERSON values(" +
					"?, ?, ?, ?, ?, " +
					"?, ?, ?, ?, ?," +
					"?, ?, ?, ?, ?, ?)")

				stmt.setString(1, person.id)
				stmt.setString(2, person.openid)
				stmt.setString(3, person.nickname)
				stmt.setString(4, person.face)
				stmt.setString(5, person.description)
				stmt.setString(6, person.homepage)
				stmt.setString(7, person.locationName)
				stmt.setString(8, person.locationTimeZone)
				stmt.setString(9, person.me2dayHome)
				stmt.setString(10, person.rssDaily)
				stmt.setString(11, person.invitedBy)
				stmt.setInt(12, person.friendsCount)
				stmt.setInt(13, person.totalPosts)
				stmt.setString(14, person.updated)
				stmt.setString(15, person.registered)
				stmt.setString(16, concatStringList(person.friendsList))

				try {
					stmt.executeUpdate()
					connection.commit()
					println("insertion success: " + person.id)
				} catch {
					case e: Throwable =>
						e.printStackTrace();
						println("insertion fail")
				} finally {
					stmt.close()
				}
		}
	}

	def getDesc(userName: String): String = {
		val stmt = connection.prepareStatement("SELECT * FROM Me2Person WHERE id=?")
		stmt.setString(1, userName)

		val rs = stmt.executeQuery()
		var desc = ""
		rs.next()
		desc = rs.getString("friendsList")
		stmt.close()
		desc
	}

	def getPerson(userName: String): Option[Me2Person] = {
		val stmt = connection.prepareStatement("SELECT * FROM Me2Person WHERE id= ? ")
		stmt.setString(1, userName)
		val rs = stmt.executeQuery()
		val isFound = rs.next()
		if (isFound == false) {
			stmt.close()
			return None
		} else {
			val person = new Me2Person(rs.getString(1),
				rs.getString(2),
				rs.getString(3),
				rs.getString(4),
				rs.getString(5),
				rs.getString(6),
				rs.getString(7),
				rs.getString(8),
				rs.getString(9),
				rs.getString(10),
				rs.getString(11),
				rs.getInt(12),
				rs.getInt(13),
				rs.getString(14),
				rs.getString(15),
				splitString(rs.getString(16)))
			stmt.close()
			return Some(person)
		}
	}

	def updateFriendsList(userName: String, newFriendName: String): Unit = {
		val thisPerson = getPerson(userName)
		thisPerson match {
			case Some(person) => {
				if (person.friendsList.contains(newFriendName) == false) {
					val newList = newFriendName :: person.friendsList
					//person.friendsList = newList sort ((x, y) => x < y)
					//println("updated  Friend list here: " + person.friendsList)
					updatePerson(person)
				}
			}
			case None => {
				val newPerson = Me2Person.makePerson(userName)
				newPerson match {
					case Some(person) => updatePerson(person)
					case None =>
				}
			}
		}
	}

	def splitString(words: String): List[String] = {
		if (words == "") {
			return List[String]()
		} else {
			try {
				if (words.charAt(0) == ' ') {
					println("getperson error! friends list start with an whitespace")
					return (words.substring(1).split(" ").toList)
				} else {
					return words.split(" ").toList
				}
			} catch {
				case _: Throwable => { // null string
					//println(words) 
					return List[String]()
				}
			}
		}

	}

	def concatStringList(wordList: List[String]): String = {
		if (wordList.length == 0) {
			return ""
		}
		var builder = new StringBuilder();
		for (word <- wordList) {
			builder.append(word)
			builder.append(" ")
		}
		builder.deleteCharAt(builder.length - 1) // remove last ' '
		builder.toString
	}

	def truncate: Unit = {
		val tableName = "TEST"
		val insertStmt = connection.createStatement()
		try {
			insertStmt.executeUpdate("Truncate table " + tableName)
			print("Truncate OK\n");
		} catch {
			case _: Throwable => print("trunc fail");
		}
	}

	def pushData() {
		val tableName = "TEST"
		Thread.sleep(1000)
		val stmt = connection.prepareStatement(
			"INSERT into " + tableName + " values (?, ?)")
		val longString = "남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅남기웅"
		val shortString = "남기웅남기웅남기웅남기웅남기웅남기웅"
		val wordString = "남기웅"
		val startTime: Double = System.currentTimeMillis()
		val testSize = 100000
		val rowSize = 200
		val batchSize = 20;
		for (i <- 1 to testSize) {
			stmt.setInt(1, i)
			stmt.setString(2, shortString + i)
			stmt.addBatch()
			if (i % batchSize == 0) {
				stmt.executeBatch()
				connection.commit
			}
			if (i % 10000 == 0) {
				println("Now done " + i / 1000 + " k'th row")
			}
		}
		val endTime = System.currentTimeMillis()
		val timeConsumed = testSize / ((endTime - startTime) / 1000.0)
		println("TPS:\t" + timeConsumed / 1000.0 + " k / sec")
		//println("Throughput:\t" + timeConsumed * rowSize / 1000000 + " MB / sec")
	}
}

