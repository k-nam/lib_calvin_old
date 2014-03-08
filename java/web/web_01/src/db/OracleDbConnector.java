package db;

import java.sql.*;

public class OracleDbConnector {
	
	private static void loadDriver() throws ClassNotFoundException {

			Class.forName(oracleDriverName);
			isDriverLoaded = true;
	}
	
	public static Connection getConnection() {
		try {
			if (isDriverLoaded == false) {
				loadDriver();
			}
			String connectionString = "jdbc:oracle:thin:" + 
									  userId + "/" + userPassword + "@//" +
									  dbHostName + ":" + dbPort + "/" + oracleSid;
			return DriverManager.getConnection(connectionString);
		} catch (Exception e) {
			System.out.println("Oracle DB connection error");
			return null;
		}
	}
	
	private static final String oracleDriverName = "oracle.jdbc.OracleDriver";
	private static final String userId = "webdev";
	private static final String userPassword = "calvin1729";
	private static final String dbHostName = "localhost";
	private static final String dbPort = "1521";
	private static final String oracleSid = "dev";
	
	private static boolean isDriverLoaded = false;
	
}
