package test

import db._
import crawler._

object DbTest {

	def main(args: Array[String]): Unit = {

		//println(getTotalLinks())
		//htmlCrawlerTest()  
		dbManagerTest()
		//removeSpaceInFriendsList()
		//getNewFriendsTest()
	}

	def dbManagerTest() {
		//val dbManager = new MysqlDbManager()
		//val dbManager = new SqlServerDbManager()
		var dbManagers = List[DbManager]()
		for (i <- 1 to 4) {
			dbManagers = new SqlServerDbManager() :: dbManagers
		}
		dbManagers(0).truncate
		for (dbManager <- dbManagers) {
			dbManager.start
		}
		for (dbManager <- dbManagers) {
			dbManager.join
		}
		//println(dbManager.getDesc("nkw0705")) 
		//val test = "2009-12-11T02:00:59Z"
		//println (test)
		//println (dbManager.getOnlyDigits(test))
	}

	def getTotalLinks(): Int = {
		val dbManager = new MysqlDbManager()
		val idSet = dbManager.getPersonList()
		var sumTotal: Int = 0
		for (id <- idSet) {
			val thisPerson = dbManager.getPerson(id)
			thisPerson match {
				case Some(person) => sumTotal += person.friendsList.length
				case None =>
			}
		}
		return sumTotal
	}

	def removeSpaceInFriendsList() {
		val dbManager = new MysqlDbManager()
		val idList = dbManager.getPersonList()
		for (id <- idList) {
			val thisPerson = dbManager.getPerson(id)
			thisPerson match {
				case Some(person) => {
					val friendsSet = Set[String]() ++ person.friendsList
					if (friendsSet.contains("") == true) {
						println(person.friendsList.length)
						println(id + " had space!!")
						person.friendsList = (friendsSet - "").toList.sortWith((x, y) => x < y)
						dbManager.updatePerson(person)
						println(person.friendsList)
						println(person.friendsList.length)
					}
				}
				case None => {
				}
			}
		}
	}

	def getNewFriendsTest() {
		val dbManager = new OracleDbManager()
		val newFriends: List[String] = dbManager.getNewPeple()
		println("new friend size is " + newFriends.length)
		for (newFriend <- newFriends) {
			val me2person = Me2Person.makePerson(newFriend)
			me2person match {
				case Some(person) => dbManager.updatePerson(person)
				case None => None
			}
		}
	}

}
