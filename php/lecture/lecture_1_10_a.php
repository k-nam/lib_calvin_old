<html>
<body>
<?php
	// 반복되는 내용을 표시하기 echo 문을 여러번 사용해 보자
	echo "수동으로 한 결과:<br>";
	echo "1번째 줄<br>";
	echo "2번째 줄<br>";
	echo "3번째 줄<br>";
	
	echo "while 문을 이용한 결과:<br>";
	$line_number = 1;
	$number_of_total_lines = 3;
	while ($line_number <= $number_of_total_lines) {
		echo "".$line_number."번째 줄<br>";
		$line_number += 1;
	}
?>
</body>
</html>