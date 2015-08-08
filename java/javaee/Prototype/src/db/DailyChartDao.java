package db;

import java.sql.Connection;
import java.util.Date;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

import bean.DailyStatus;

public class DailyChartDao {
	public static List<DailyStatus> getDailyChart() {
		List<DailyStatus> result = new ArrayList<DailyStatus>();
		Connection connection = SqlServerDbConnector.getConnection();
		try {
			String query = "SELECT (*) FROM PatientChart.DailyStatus;";
			PreparedStatement pstmt = connection.prepareStatement(query);
			ResultSet rs = pstmt.executeQuery();
			while (rs.next()) {
				Date a = rs.getDate("date");
				Timestamp b = rs.getTimestamp("recordedTime");
				int mood = rs.getInt("mood");
				int medication = rs.getInt("medication");
				result.add(new DailyStatus(a, b, mood, medication));
			}
			connection.close();
			return result;
		} catch (SQLException e) {
			e.printStackTrace();
			return null;
		}
	}
}
