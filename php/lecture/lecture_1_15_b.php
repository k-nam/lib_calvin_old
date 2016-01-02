<html>
<body>
<?php	
	// 피보나치 함수의 n 항 구하기
	// 편의상 인자가 3 이상이라고 가정하자.
	function fibonacci($number) {
		$previous = 1; // index-1 번째 항의 값
		$current = 1; // index 번째 항의 값
		// for 문이 시작되기 직전 index는 2이다.
		for ($index = 3; $index <= $number; $index += 1) {
			$sum = $previous + $current;
			$previous = $current;
			$current = $sum;
		}
		return $current;
	}
	
	function fibonacciByRecursion($number) {
		if ($number <= 2) {
			return 1;
		} else {
			return fibonacciByRecursion($number - 1) + fibonacciByRecursion($number - 2);
		}
	}
	echo "피보나치 수열의 20번째 값은?<br>";
	echo "for 문으로 구한 값 = ".fibonacci(20)."<br>";
	echo "재귀함수로 구한 값 = ".fibonacciByRecursion(20)."<br>";
?>

</body>
</html>