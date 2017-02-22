package db;

import java.sql.Connection;
import java.sql.DriverManager;

public class MysqlDbConnector {
	private static void loadDriver() throws ClassNotFoundException {
		Class.forName(mysqlDriverName);
		isDriverLoaded = true;
	}

	public static Connection getConnection(String databaseName) {
		String connectionString = "jdbc:mysql://" + dbHostName + ":" + dbPort  + 
				"/" + databaseName;
		try {
			if (isDriverLoaded == false) {
				loadDriver();
			}
			return DriverManager.getConnection(connectionString, userId, userPassword);
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println(connectionString);
			System.out.println("Mysql DB connection error");
			return null;
		}
	}
	
	public static Connection getConnection() { 
		return getConnection(databaseName);
	}
	
	private static final String mysqlDriverName = "com.mysql.jdbc.Driver";
	private static final String userId = "root";
	private static final String userPassword = "password";
	private static final String dbHostName = "calvin-mysql.cytzyor3ndjk.ap-northeast-2.rds.amazonaws.com";
	private static final String dbPort = "3306";
	private static final String databaseName = "mywebsite_prototype";
	private static boolean isDriverLoaded = false;
}
