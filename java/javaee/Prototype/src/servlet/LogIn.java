package servlet;

import java.io.IOException;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

/**
 * Servlet implementation class LogIn
 */
public class LogIn extends HttpServlet {
	private static final long serialVersionUID = 1L;

	/**
	 * @see HttpServlet#HttpServlet()
	 */
	public LogIn() {
		super();
		// TODO Auto-generated constructor stub
	}

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse
	 *      response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse
	 *      response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {

		String userId = request.getParameter("userid");
		String userPassword = request.getParameter("userpassword");
		System.out.println("In servlet: " + userId + " " + userPassword);
		boolean isLoginSuccessful = logic.Login.logIn(userId, userPassword);
		if (isLoginSuccessful) {
			HttpSession curSession = request.getSession();
			curSession.setAttribute("isLoggedIn", "true");
			bean.User user = new bean.User(userId, "hisName");
			String[] hobbies = { "gender", "programming", "reading" };
			user.setHobbies(hobbies);
			curSession.setAttribute("user", user);
			RequestDispatcher view = request.getRequestDispatcher("/practice/index.jsp");
			view.forward(request, response);
		} else {

		}

	}

}
