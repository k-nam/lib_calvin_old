package servlet.patientchart;

import java.io.IOException;
import java.sql.Date;
import java.sql.Timestamp;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import bean.DailyStatus;

import com.google.gson.Gson;

import db.DailyChartDao;

/**
 * Servlet implementation class DailyChart
 */
public class DailyChart extends HttpServlet {
	private static final long serialVersionUID = 1L;

	/**
	 * @see HttpServlet#HttpServlet()
	 */
	public DailyChart() {
		super();
		// TODO Auto-generated constructor stub
	}

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse
	 *      response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		System.out.println("Now in get!");
		List<DailyStatus> list = DailyChartDao.getChart();
		Gson gson = new Gson();
		String json = gson.toJson(list);
		response.getWriter().append(json);
	}

	// record new info onto chart
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		System.out.println("Now in post!");
		String argument = request.getParameter("arg"); // JSON representation of bean argument
		Gson gson = new Gson();
		DailyStatus bean = gson.fromJson(argument, DailyStatus.class);
		System.out.println("arg was: <" + argument + ">");
		DailyChartDao.updatChart(new Date(bean.date), new Timestamp(bean.recordedTime), bean.mood, bean.medication);
	}

}
