package db;

import java.sql.Connection;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

import bean.DailyStatus;

public class DailyChartDao {
	public static List<DailyStatus> getChart() {
		List<DailyStatus> result = new ArrayList<DailyStatus>();
		try {
			Connection connection = MysqlDbConnector.getConnection();
			String query = "SELECT date, recorded_time, mood, medication FROM patient_chart;";
			PreparedStatement pstmt = connection.prepareStatement(query);
			ResultSet rs = pstmt.executeQuery();
			while (rs.next()) {
				Date a = rs.getDate("date");
				Timestamp b = rs.getTimestamp("recorded_time");
				int mood = rs.getInt("mood");
				int medication = rs.getInt("medication");
				result.add(new DailyStatus(a.getTime(), b.getTime(), mood, medication));
			}
			connection.close();
			return result;
		} catch (SQLException e) {
			e.printStackTrace();
			return null;
		}
	}

	public static void updatChart(Date date, Timestamp recordedTime, int mood, int medication) {
		try {
			Connection connection = MysqlDbConnector.getConnection();
			String query = "INSERT INTO patient_chart (date, recorded_time, mood, medication)" +
			" VALUES (?, ?, ?, ?);";
			PreparedStatement pstmt = connection.prepareStatement(query);
			pstmt.setDate(1, date);
			pstmt.setTimestamp(2, recordedTime);
			pstmt.setInt(3, mood);
			pstmt.setInt(4, medication);
			pstmt.execute();
			connection.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
}
