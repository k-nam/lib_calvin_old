<?php
function test_input($data) {
	$data = trim($data);
	$data = stripslashes($data);
	$data = htmlspecialchars($data);
	return $data;
}
function removeNewline($string) {
	return preg_replace('~[\r\n]+~', '', $string);;
}
function debugLog($string) {
	$debugFileName = "debug.log";
	file_put_contents($debugFileName, "Debuglog: <", FILE_APPEND);
	file_put_contents($debugFileName, $string, FILE_APPEND);
	file_put_contents($debugFileName, ">\n", FILE_APPEND);
}

function getDbConnection() {
	//return getMysqlConnection();
	return getMssqlConnection();
}

function getMysqlConnection() {
	$servername = "localhost:3306";
	$username = "root";
	$password = "calvin1729";
	$conn = new mysqli($servername, $username, $password);
	if ($conn->connect_error) {
		die("Mysql connection failed: " . $conn->connect_error);
	} else {
		// if echo in a function, it becomes the return value
	}
	return $conn;
}

function getMssqlConnection() {
	$servername = "localhost";
	$database = "Web";
	$username = "sa";
	$password = "calvin1729";
	try {
		$handle = new PDO("sqlsrv:Server=".$servername.";Database=".$database.";", $username, $password);
		return $handle;
	} catch (PDOException $e) {
		echo "Mssql connection failed";
		$e->getMessage();
	}
}

?>
