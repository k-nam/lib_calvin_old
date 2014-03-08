package crawler

import db._;
import scala.List;

class Me2Crawler(seed: String, max: Int, dbManager: DbManager) extends Thread {

	override def run() {
		crawl(seed, max, dbManager)
	}

	def crawl(seed: String, max: Int, dbManager: DbManager): Unit = {

		val htmlCrawler = new HtmlCrawler()
		//var map = Map[String, Set[String]]() // username -> usernames of his friends 
		// DFS with large stack (does not use proper priority queue type stack)
		// O(E) size of stack instead of O(V)
		var stack = new scala.collection.mutable.Stack[String]
		stack.push(seed)
		Me2Crawler.discoveredUsers += seed
		var numOps = 0
		while (stack.size != 0 && numOps < max) {

			val userName = stack.pop()
			if (Me2Crawler.finishedUsers.contains(userName)) {
				println("already done user")
			} else {
				println("creating user: " + userName)
				createPerson(userName)
				//updateFriendsList()
				Me2Crawler.finishedUsers += userName
			}

				def createPerson(userName: String) {
					val result = Me2Person.makePerson(userName)
					result match {
						case Some(person) => {
							numOps += 1
							//println ("I am " + seed + " dicovered total:" + 
							//Me2Crawler.discoveredUsers.size + "  users")
							//println(person.toString)
							dbManager.updatePerson(person)
							//println ("found user: " + userName + " queuesize is: " + 
							//stack.size + " num friend: " + person.friendsList.size + "numop=" + numOps)

							for (friend <- person.friendsList) {
								if (Me2Crawler.discoveredUsers.contains(friend) == false) {
									Me2Crawler.discoveredUsers += friend
									stack.push(friend)
								}
							}
						}
						case None => println("me2day API failed,,")
					}
				}

				def updateFriendsList() {
					val recentFriendsList = htmlCrawler.getRecentFriendsList(userName)
					val thisPerson = dbManager.getPerson(userName)

					thisPerson match {
						case Some(person) => { // update friends list if needed
							println(person.friendsList)
							var curList: Set[String] = Set[String]() ++ person.friendsList
							var needToUpdate = false
							for (recentFriend <- recentFriendsList) {
								if (curList.contains(recentFriend) == false) {
									println("found new friend of " + userName + " : " + recentFriend)
									needToUpdate = true
								}
								// Add the edge for the other way
								dbManager.updateFriendsList(recentFriend, userName)
							}
							if (curList.contains("") == true) {
								println("ERROR: " + userName + "this user contains \' \' !! deleting")
							}
							if (needToUpdate) {
								curList ++= recentFriendsList
								person.friendsList = curList.toList.sortWith((x, y) => x < y)
								dbManager.updatePerson(person)
							}

							// Working queue management
							for (friend <- curList) {
								if (Me2Crawler.discoveredUsers.contains(friend) == false) {
									Me2Crawler.discoveredUsers += friend
									stack.push(friend)
								}
							}
						}
						case None => { // newly discoverd user!
							//createPerson()
						}
					}
				}
			None
		}
		println("\n\n\n-------CRAWLER QUEUE EMPTIED---------\n\n\n")
	}
}

object Me2Crawler {
	var discoveredUsers = Set[String]() // mark discovered user(=grey nodes)
	var finishedUsers = Set[String]() // black nodes

}

