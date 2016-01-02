<html>
<body>
<?php
	$cost_total = 60000;
	$man_ratio_for_dating_cost = 0.6;
	$money_to_pay = 0; // 아직 어떤 값인지 모르니까
	$sex = "남자";
	if ($sex == "남자") {
		$money_to_pay = $cost_total * $man_ratio_for_dating_cost;
	}
?>
오늘의 총 데이트 비용은 6만원이었다. 데이트 비용의 60%는 남자가 내는 것이 적당하다고 가정하자. 당신이 <?php echo $sex; ?>라면, 오늘 내야 할 돈은 <?php echo $money_to_pay; ?>원이다.
</body>
</html>