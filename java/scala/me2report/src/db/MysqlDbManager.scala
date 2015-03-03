package db

import java.sql.{ DriverManager, Connection };
import lib.db.connection._

class MysqlDbManager extends DbManager {
	val connection: Connection = new MysqlConnection("test").connection
}