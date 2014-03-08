package jdbc;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;

import oracle.jdbc.OracleConnection;
import oracle.jdbc.OraclePreparedStatement;

public class JdbcSample {
	public void test() {
		Connection mySqlConn = null;
		OracleConnection oracleConn = null;
		PreparedStatement mySqlStmt = null;
		OraclePreparedStatement oracleStmt = null;
		// ResultSet rs = null;

		try {
			Class.forName("com.mysql.jdbc.Driver");
			Class.forName("oracle.jdbc.OracleDriver");
		} catch (Exception e) {
			System.out.println("Not Found JDBC Driver");
			return;
		}

		try {
			mySqlConn = DriverManager.getConnection("jdbc:mysql://localhost/mysql?"
					+ "user=root&password=calvin1729&characterEncoding=UTF-8");
			oracleConn = (OracleConnection) DriverManager
					.getConnection("jdbc:oracle:thin:calvin/calvin1729@//localhost:1521/me2day");
			mySqlConn.setAutoCommit(false);
			// Employee2: Id, Name(sting), data1, data2, ... data10
			mySqlStmt = mySqlConn
					.prepareStatement("INSERT INTO Employee2 VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
			// oracleStmt = (OraclePreparedStatement) oracleConn.prepareStatement(
			// "INSERT INTO Employee2 VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
			oracleStmt = (OraclePreparedStatement) oracleConn
					.prepareStatement("INSERT INTO table1 VALUES (?, ?)");
			oracleConn.setDefaultExecuteBatch(10000);
			oracleConn.setAutoCommit(false);
			System.out.println("jdbc connection success");

			for (int i = 0; i < 100000; i++) {
				try {
					mySqlStmt.setInt(1, i);
					mySqlStmt.setString(2, "employee");
					mySqlStmt.setInt(3, i);
					mySqlStmt.setInt(4, i);
					mySqlStmt.setInt(5, i);
					mySqlStmt.setInt(6, i);
					mySqlStmt.setInt(7, i);
					mySqlStmt.setInt(8, i);
					mySqlStmt.setInt(9, i);
					mySqlStmt.setInt(10, i);
					mySqlStmt.setInt(11, i);
					mySqlStmt.setInt(12, i);
					// mySqlStmt.addBatch();

					oracleStmt.setInt(1, i);
					oracleStmt.setString(2, "employee");
					/*
					 * oracleStmt.setInt(3, i); oracleStmt.setInt(4, i);
					 * oracleStmt.setInt(5, i); oracleStmt.setInt(6, i);
					 * oracleStmt.setInt(7, i); oracleStmt.setInt(8, i);
					 * oracleStmt.setInt(9, i); oracleStmt.setInt(10, i);
					 * oracleStmt.setInt(11, i); oracleStmt.setInt(12, i);
					 */
					oracleStmt.executeUpdate();
					if (i % 10000 == 0) {
						mySqlStmt.executeBatch();
						mySqlConn.commit();
						oracleConn.commit();
						System.out.println("Now inserted " + i + " rows");
					}
				} catch (SQLException e) {
					e.printStackTrace();

				}
			}
			/*
			 * rs = mySqlStmt.executeQuery("select Id, Name from Employee2;"); int
			 * count = 0; while(rs.next() == true) { count++; String a =
			 * "Employee id is " + rs.getInt("Id"); String b = "name is " +
			 * rs.getString("Name"); }
			 */
			// System.out.println("total record is: " + count);
		} catch (SQLException e) {
			e.printStackTrace();
			System.out.println("jdbc error");
		}
	}
}
