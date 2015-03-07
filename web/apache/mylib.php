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
?>