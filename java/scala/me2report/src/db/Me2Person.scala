package db

import scala.xml._;
import java.io._;
import java.net.URL;

class Me2Person(val id: String, var openid: String, var nickname: String,
		var face: String, var description: String, var homepage: String,
		var locationName: String, var locationTimeZone: String, var me2dayHome: String,
		var rssDaily: String, var invitedBy: String, var friendsCount: Int,
		var totalPosts: Int, var updated: String, var registered: String,
		var friendsList: List[String]) {

	override def toString(): String = {
		"id is " + id + " nickname is " + nickname + " friendsCount is " +
			friendsCount.toString()
	}
}

object Me2Person {
	def makePerson(userName: String): Option[Me2Person] = {
		println("api|")
		try {
			val personUrl = new URL("http://me2day.net/api/get_person/" + userName + ".xml")
			val friendsUrl = new URL("http://me2day.net/api/get_friends/" + userName + ".xml")
			Some(makePerson(personUrl.openStream(), friendsUrl.openStream()))
		} catch {
			case _ => None
		}
	}

	def getOnlyDigits(timestamp: String): String = {
		if (timestamp == null) {
			return null
		}
		//println("calling getonlydigits on: " + timestamp)   
		val charList = timestamp.toList
		val digits = List.toString()
		//println(digits + " utc")
		if (digits.length() < 3) {
			println("This wrong timestamp was " + timestamp)
			return ""
		} else {
			return digits
		}
	}

	def makePerson(person: Node, friendsList: List[String]): Me2Person = {
		val newPerson = new Me2Person((person \\ "id").text,
			(person \\ "openid").text,
			(person \\ "nickname").text,
			(person \\ "face").text,
			(person \\ "description").text,
			(person \\ "homepage").text,
			(person \\ "name").text,
			(person \\ "timezone").text,
			(person \\ "me2dayHome").text,
			(person \\ "rssDaily").text,
			(person \\ "invitedBy").text,
			(person \\ "friendsCount").text.toInt,
			(person \\ "totalPosts").text.toInt,
			getOnlyDigits((person \\ "updated").text),
			getOnlyDigits((person \\ "registered").text),
			friendsList)

		newPerson
	}

	def getFriendsList(friendsListXml: InputStream): List[String] = {
		try {
			val xmlElem = XML.load(friendsListXml)
			val ids = xmlElem \\ "id"
			val idList = (for (id <- ids) yield id.text).toList
			return idList
		} catch {
			case _ => return List()
		}
	}

	def makePerson(personXml: InputStream, friendsListXml: InputStream): Me2Person = {
		makePerson((XML.load(personXml) \\ "person")(0), getFriendsList(friendsListXml))
	}

}
