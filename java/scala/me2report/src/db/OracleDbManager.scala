package db

import java.sql.DriverManager;
import oracle.jdbc.OracleConnection

class OracleDbManager extends DbManager {
	val connection: OracleConnection = new lib.db.connection.OracleConnection("").connection
}

