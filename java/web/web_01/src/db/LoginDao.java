package db;

import java.sql.*;


public class LoginDao {

	public static boolean logIn(String userId, String userPassword) {
		Connection connection = OracleDbConnector.getConnection();
		if (connection == null) {
			System.out.println("Oracle db connection error");
			return false;
		}
		int numFound;
		try {
			PreparedStatement pstmt = 
				connection.prepareStatement(
						"SELECT COUNT(1) FROM useraccounts " +
						"WHERE id = ? AND password = ?");
			pstmt.setString(1, userId);
			pstmt.setString(2, userPassword);
			ResultSet rs = pstmt.executeQuery();
			rs.next();
			numFound = rs.getInt(1);
			System.out.println("id: " + userId + " passwd: " + userPassword);
			System.out.println("numfound is: " + numFound);
			connection.close();
		} catch (SQLException e) {
			e.printStackTrace();	
			return false;
		} 
		if (numFound > 1) {
			System.out.println("Internal error: multiple accounts with" +
					"identical userid and password!");
			return false;
		} else if (numFound == 1) {
			return true;
		} else if (numFound == 0) {
			return false;
		} else {
			System.out.println("Internal error: db query error");
			return false;
		}
	}	
}
