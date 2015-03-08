package db;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class DictionaryDao {
	public static List<String> getAutocompleteSuggestions(String prefix, int num) {
		List<String> result = new ArrayList<String>();
		Connection connection = SqlServerDbConnector.getConnection();
		try {
			String query = "IF OBJECT_ID('tempdb.dbo.#temp') IS NOT NULL DROP TABLE #temp;"
					+ "SELECT TOP "
					+ Integer.valueOf(num)
					+ " word, rank INTO #temp "
					+ "FROM Wordlist.Eng_100K WHERE word LIKE '"
					+ prefix
					+ "%' "
					+ "ORDER BY  (CASE WHEN rank IS NULL THEN 1 ELSE 0 END), rank ASC;"
					+ "SELECT word, rank FROM #temp ORDER BY word;";
			PreparedStatement pstmt = connection.prepareStatement(query);
			ResultSet rs = pstmt.executeQuery();
			while (rs.next()) {
				result.add(rs.getString(1));
			}
			connection.close();
			return result;
		} catch (SQLException e) {
			e.printStackTrace();
			return null;
		}

	}
}
