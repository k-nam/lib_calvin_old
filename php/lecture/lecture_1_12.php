<html>
<body>
<?php
	$data_total = 0;
	$data_per_day = 100;
	$data_increase_per_day = 10;
	for ($day = 1; $day <= 30; $day += 1) {
		$data_total += $data_per_day; 
		$data_per_day += $data_increase_per_day; 
	}
?>
김군은 눈 만 떳다 하면 스마트폰을 손에서 떼지 못하는 중독자다. 하지만 사용할 수 있는 데이터량에는 한계가 있기 때문에, 매달 초에는 항상 "이번달은 진짜 적게 써야지." 하고 결심을 한다. 하지만 그 결심은 항상 무너지고 말아서, 하루하루가 지날수록 조금씩 인터넷을 더 많이 하고 만다.<br><br>
계량화 해 본 결과, 김군은 매달 첫날에는 100메가바이트를 사용하고, 그 이후로는 하루에 10메가바이트씩 더 많이 사용한다고 가정하자. 즉 매달 2일에는 110메가바이트를, 3일에는 120메가바이트를 사용하는 식이다. <br><br>
이번 달이 6월이라고 가정하면, 김군이 이번 한 달 동안 쓰게될 총 데이터량은 얼마일까? 반복문을 써서 답을 구해보자!<br><br>
답: <?php echo $data_total; ?>메가바이트<br><br>

여름철을 지내면서 김군의 스마트폰 중독이 더욱 심해져서, 매일 증가하는 데이터 사용량이 10메가바이트가 아닌 15메가바이트가 되었다고 한다. 9월 한달동안 쓰게될 총 데이터량은 얼마일까?<br><br>
<?php
	$data_total = 0;
	$data_per_day = 100;
	$data_increase_per_day = 15; // 변경된 값은 딱 이거 하나뿐이다!
	for ($day = 1; $day <= 30; $day += 1) {
		$data_total += $data_per_day; 
		$data_per_day += $data_increase_per_day;
	}
?>
답: <?php echo $data_total; ?>메가바이트<br><br>
</body>
</html>