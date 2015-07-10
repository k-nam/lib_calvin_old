package servlet;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import logic.Planner;

/**
 * Servlet implementation class select
 */
public class SelectSubject extends HttpServlet {
	private static final long serialVersionUID = 1L;

	/**
	 * @see HttpServlet#HttpServlet()
	 */
	public SelectSubject() {
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
		response.setContentType("text/html");
		PrintWriter out = response.getWriter();
		out.println("select servlet OK!");
		int age = Integer.parseInt(request.getParameter("age"));
		String recommendedSubject = new Planner().recommendSubject(age);
		// out.println("we recommend you study: " + recommendedSubject + " now");

		request.setAttribute("recommendedSubject", recommendedSubject);
		request.setAttribute("hobbies", request.getParameterValues("hobby"));
		request.setAttribute("paramName",
				getServletConfig().getInitParameter("paramName"));
		request.setAttribute("dao", getServletContext().getAttribute("dao"));

		RequestDispatcher view = request.getRequestDispatcher("/practice/select.jsp");
		view.forward(request, response);
	}

}
