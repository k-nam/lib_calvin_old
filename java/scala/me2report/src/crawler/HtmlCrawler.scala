package crawler

import java.io._
import java.net.URL
import scala.util.matching._

object HtmlCrawler {
	//ddd
	val bufferSize = 10000

}

class HtmlCrawler {
	val buffer: Array[Byte] = new Array[Byte](HtmlCrawler.bufferSize)

	def getRecentFriendsList(userId: String): List[String] = {
		try {
			val html = printHtml("http://me2day.net/" + userId + "/friends/all")
			getFriendsList(html)
		} catch {
			case _ => return List()
		}
	}

	def printHtml(urlString: String): String = {
		//println("<<<<< sending html request >>>> " + urlString)
		print("html|")
		val personUrl = new URL(urlString)
		printStream(personUrl.openStream())
	}

	def printStream(in: InputStream): String = {
		val numBytesRead = in.read(buffer)
		if (numBytesRead == -1) {
			new String()
		} else {
			new String(buffer, 0, numBytesRead, "UTF-8") + printStream(in)
		}
	}

	def getFriendsList(friendsHtml: String): List[String] = {
		val friendsIdPattern = new Regex("(?s)<div class=\"sec_post\">.*?" +
			"<div class=\"post_section no_border\">.*?" +
			"<div class=\"post_cont\">.*?" +
			"<a href=\"/(.+?)/.*?\" rel=\".*?\" title=\".*?\">")
		val b = friendsIdPattern findAllIn friendsHtml
		var result = List[String]()

		while (b.hasNext) {
			if (isNormalId(b.group(1))) {
				result = b.group(1) :: result
			}
			b.next
		}
		val resultSet = Set[String]() ++ result
		return resultSet.toList

	}

	def isNormalId(id: String): Boolean = {
		if (id == "" || id.charAt(0) == ' ') {
			return false;
		} else {
			return true;
		}
	}
}

