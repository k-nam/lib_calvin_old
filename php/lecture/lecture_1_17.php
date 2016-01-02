<html>
<body>
<?php		
	$days1 = array("월요일", "화요일", "수요일");
	$days2 = array("목요일", "금요일", "토요일");
	$days3 = array("일요일");
	
	// array_merge 로 어레이들을 합친다
	$days = array_merge($days1, $days2, $days3);
	
	foreach($days as $key => $value) { 
		echo $key;
		echo ":";
		echo $value;
		echo " "; 
	}
	echo "<br>";
	
	echo "어레이의 길이는 ".count($days)."이다";
?>

</body>
</html>