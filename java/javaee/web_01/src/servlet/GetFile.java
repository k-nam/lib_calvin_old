package servlet;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Enumeration;

import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class getTextData
 */
public class GetFile extends HttpServlet {
	private static final long serialVersionUID = 1L;

	/**
	 * @see HttpServlet#HttpServlet()
	 */
	public GetFile() {
		super();
		// TODO Auto-generated constructor stub
	}

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse
	 *      response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		if (logic.Login.isLoggedIn(request) == false) {
			PrintWriter writer = response.getWriter();
			writer.write("you should be logged in to download!");
			return;
		}

		response.setContentType("application/jar");
		
		ServletContext ctx = getServletContext();
		Enumeration<String> enumeration = ctx.getAttributeNames();
		while (enumeration.hasMoreElements()) {
			String attribute = enumeration.nextElement();
			//System.out.println(attribute + " " + ctx.getAttribute(attribute));
		}
		
		InputStream in = ctx.getResourceAsStream("/abc.jar");
		if (in == null) {
			System.out.println("a1");
			// return;
		}

		int numBytesRead = 0;
		byte[] buffer = new byte[1024];
		OutputStream out = response.getOutputStream();

		while (true) {
			numBytesRead = in.read(buffer);
			if (numBytesRead == -1) {
				break;
			}
			out.write(buffer, 0, numBytesRead);
		}
		out.flush();
		out.close();
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse
	 *      response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
	
		// TODO Auto-generated method stub
	}

}
