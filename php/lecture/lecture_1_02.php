<html>
<body>
<?php	
	// 올바른 변수명의 예시
	$instructor_name = "남기웅";
	$current_season = "봄";
	$number_of_students_in_class = 15;
	$html_file_name = "index.html"; // html은 보편적으로 사용하는 줄임말
	$is_alive = true;
	
	// 좋지 않은 변수명의 예시
	$instructer_name = "남기웅"; // 철자가 틀림
	$instructor_name = 1; // 값의 타입과 변수명이 암시하는 타입이 달라서 혼동스럽다.
	$i = 15; // 15 라는 값의 의미를 알 수 없다. 
	$nosic = 15; // 자기 맘대로 단어를 축약함.
	$isDead = false; // 이 파일에서 사용하고 있는 변수명 스타일과 다르다.
	
?>
</body>
</html>