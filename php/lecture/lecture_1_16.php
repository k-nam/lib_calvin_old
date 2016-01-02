<html>
<body>
<?php	
	echo "월요일 ";
	echo "화요일 ";
	echo "수요일 ";
	echo "<br>";
	
	$days = array("월요일", "화요일", "수요일");
	
	// foreach 문은 for문과 유사하고, 어레이의 모든 값을 순서대로 반복한다.
	foreach($days as $key => $value) { 
		echo $key;
		echo ":";
		echo $value;
		echo " "; // 단어 사이의 띄어쓰기를 간단히 처리할 수 있다.
	}
	echo "<br>";
	
	// 대괄호 [] 는 어레이의 원하는 위치의 값을 찾아준다
	for ($day = 0; $day < 3; $day += 1) {
		echo $days[$day]; 
		echo " ";
	}
?>

</body>
</html>