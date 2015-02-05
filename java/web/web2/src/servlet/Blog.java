package servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class Blog extends HttpServlet {

	private static final long serialVersionUID = -867504708028091886L;

	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		System.out.println("In Blog servlet");
		String keyword = request.getParameter("keyword");
		if (response == null) {
			System.out.println("Response is null");
		}
		if (response.getWriter() == null) {
			System.out.println("Response writer is null");
		}

		response.setContentType("text");
		response.getWriter().write(keyword);
		response.getWriter().append(" 1");
		response.setStatus(200);
		System.out.println("Keyword was: " + keyword);
	}
}
