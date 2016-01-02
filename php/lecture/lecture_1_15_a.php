<html>
<body>
<?php	
	function getSumFromOneToN($number) {
		$total = 0;
		for ($current = 1; $current <= $number; $current += 1) {
			$total += $current;
		}
		return $total;
	}
	
	// 재귀를 이용한 구현
	function getSumFromOneToNByRecursion($number) {
		if ($number == 1) {
			return 1;
		} else {
			return $number + getSumFromOneToNByRecursion($number - 1);
		}
	}	
	
	echo "1에서 1000까지 더한 값은?<br>";
	echo "for 문으로 구한 값 = ".getSumFromOneToN(1000)."<br>";
	echo "재귀함수로 구한 값 = ".getSumFromOneToNByRecursion(1000)."<br>";
?>

</body>
</html>