<html>
<body>
<?php		
	$days_string = "월요일 화요일 수요일";
	$days_array = explode(" ", $days_string);
	print_r($days_array);
	echo "<br>";
	
	$new_string = implode("과 ", $days_array);
	echo $new_string;
?>

</body>
</html>