package test

import db._
import crawler._

object metoday {

	def main(args: Array[String]): Unit = {
		crawlerTest()
	}

	def crawlerTest() {
		//println (getFriends("nkw0705"))
		//println (getMutualFriends("nkw0705", "shannon"))

		//print ("found : " + map.size + " users")

		//val dbManager = new OracleDbManager()
		//val dbManager = new OracleDbManager()

		val seeds = List("shannon", "dalcrose", "kslove40",
			"lelemjh", "ayasika", "g-dradon",
			"nkw0705", "better", "pequt",
			"shannon", "dalcrose", "kslove40",
			"tok", "hyperize", "ghlab")

		var crawlers = List[Me2Crawler]()
		val max = 300000
		seeds foreach (x => crawlers =
			(new Me2Crawler(x, max, new SqlServerDbManager())) :: crawlers)
		crawlers foreach (_.start())
		crawlers foreach (_.join())
	}

	def htmlCrawlerTest() {
		val htmlCrawler = new HtmlCrawler()
		print(htmlCrawler.getRecentFriendsList("rb_jaekyung"))
	}
}
