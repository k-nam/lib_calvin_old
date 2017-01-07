package db;
import java.sql.*;
public class SqlServerDbConnector {
	private static void loadDriver() throws ClassNotFoundException {
		Class.forName(sqlServerDriverName);
		isDriverLoaded = true;
	}

	public static Connection getConnection() {
		String connectionString = "jdbc:sqlserver://" + dbHostName + ":" + dbPort + ";" + 
				"databaseName=" + databaseName;
		try {
			if (isDriverLoaded == false) {
				loadDriver();
			}
			return DriverManager.getConnection(connectionString, userId, userPassword);
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println(connectionString);
			System.out.println("SqlServer DB connection error");
			return null;
		}
	}
	
	private static final String sqlServerDriverName = "com.microsoft.sqlserver.jdbc.SQLServerDriver";
	private static final String userId = "calvin";
	private static final String userPassword = "password";
	private static final String dbHostName = "localhost";
	private static final String dbPort = "1433";
	private static final String databaseName = "Web";
	
	private static boolean isDriverLoaded = false;
}
