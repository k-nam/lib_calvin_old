<html>
<body>

<?php
require 'mylib.php';

if ($_SERVER['REQUEST_METHOD'] === 'GET') {
	$input = test_input($_GET["input"]);
	$response = "absolute"." "."beautiful"." "."commander";
	echo response;
} else {
	echo "post request???";
}

?>

</body>
</html>