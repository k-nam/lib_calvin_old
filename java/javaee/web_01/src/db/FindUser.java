package db;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class FindUser {
	public static List<String> findUserInfo(String nickname) {
		List<String> info = new ArrayList<String>();
		Connection connection = OracleDbConnector.getConnection();
		if (connection == null) {
			System.out.println("Oracle db connection error");
			return null;
		}
		try {
			PreparedStatement pstmt = 
				connection.prepareStatement(
						"SELECT id, description FROM me2day.me2person " +
						"WHERE nickname = ?");
			pstmt.setString(1, nickname);
			ResultSet rs = pstmt.executeQuery();
			while (rs.next()) {
				info.add(rs.getString(1));
				info.add(rs.getString(2));
			}
			connection.close();
			return info;
		} catch (SQLException e) {
			e.printStackTrace();	
			return null;
		} 
	}
}
