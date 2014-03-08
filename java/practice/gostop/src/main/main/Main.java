package main;



import java.util.regex.MatchResult;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import client.ClientId;

import card.Card;
import server.*;

public class Main {
	public static void main(String[] args) {
		 Pattern p = Pattern.compile("#\\w+=\\w+#");
		 Matcher m = p.matcher("where paydttm >= '#enddate=date#' and paydttm < '#enddate=date#' and regmo = '#regme=text#' group by platform;");
	

		 while (m.find()) {
			 System.out.println(m.group(0));
		 }
				 
		 //FloorCardsManager cardsOnFloor;
	}
}
