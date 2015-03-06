package db

import java.sql.{ DriverManager, Connection }; ;
import lib.db.connection._

class SqlServerDbManager extends DbManager {
	val connection: Connection = new SqlServerConnection("aaa").connection
}