package logic;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

public class Login {
	public static boolean logIn(String userId, String userPassword) {
		return db.LoginDao.logIn(userId, userPassword);
	}
	
	public static boolean isLoggedIn(HttpServletRequest request) {
		HttpSession curSession = request.getSession();
		if (((String)curSession.getAttribute("isLoggedIn")).equals("true")) {
			return true;
		} else {
			return false;
		}
	}
}
