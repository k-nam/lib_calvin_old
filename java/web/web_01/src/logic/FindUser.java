package logic;

import java.util.List;

public class FindUser {
	public static List<String> findUserInfo(String nickname) {
		List<String> result = db.FindUser.findUserInfo(nickname);
		//System.out.println("FindUser: " + result);
		return result;
	}
}
