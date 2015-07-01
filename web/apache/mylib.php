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

function getMysqlConnection() {
	$servername = "localhost:3306";
	$username = "root";
	$password = "calvin1729";
	$conn = new mysqli($servername, $username, $password);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	} 
	//echo "Connected successfully";
	return $conn;
}

?>
