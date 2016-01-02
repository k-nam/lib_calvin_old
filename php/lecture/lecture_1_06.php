<html>
<body>
<?php
	// 각 변수를 알맞은 값으로 초기화 시켜 보자.
	$cost_dinner = 25000;
	$cost_ticket = 9000;
	$cost_popcorn = 12000;
	$cost_movie = $cost_ticket * 2 + $cost_popcorn;
	$cost_coffee_per_cup = 5000;
	$cost_coffee = $cost_coffee_per_cup * 2;
?>
남녀가 데이트를 하는데 밥먹는데 25000원을 쓰고 (스파게티와 피자를 시켜서 같이 나눠 먹음), 영화보는데 30000원을 쓰고 (9000원짜리 표 두장과 팝콘세트 12000원), 커피마시는데 10000원을 썼다 (5000원짜리 두 잔과 치즈케익 하나. 치즈케익은 쿠폰으로 공짜로 먹음). <br><br>

<?php
	$man_initial_money = 40000;
	$man_final_money = 5000;
	$man_spent_money = $man_initial_money - $man_final_money;
?>
그런데 데이트가 끝나고 남자가 지갑을 뒤져보니, 꼴랑 5000원이 남아 있었다. 집에서 나올 때는 분명히 40000원이 있었는데. 그렇다면 오늘 내가 쓴 돈은 자그마치 <?php echo $man_spent_money; ?>원!<br><br>

<?php
	$cost_total = $cost_dinner + $cost_movie + $cost_coffee;
	$man_ratio_for_dating_cost = 0.5; // 이 변수는 어떤 의미가 있는가? 필요한가?

	$man_share_for_cost_dinner = $cost_dinner * $man_ratio_for_dating_cost;
	$man_share_for_cost_movie = $cost_movie * $man_ratio_for_dating_cost;
	$man_share_for_cost_coffee = $cost_coffee * $man_ratio_for_dating_cost;
	$man_share_for_cost_total = $man_share_for_cost_dinner + $man_share_for_cost_movie + $man_share_for_cost_coffee; // 다시계산 (반복)
?>
둘이서 쓴 돈이 다 합쳐서 25000 + 30000 + 10000원 (합계 <?php echo $cost_total; ?> 원) 인데 자기가 35000원을 써서 남자는 화가 났다. 남자가 말하길, "야. 내가 오늘 진짜 이런 말 안 하고 싶었는데... 오늘 우리 둘이서 총 <?php echo $cost_total; ?>원을 썻잔아. 반땡하면 <?php echo $man_share_for_cost_total; ?>원 인데 내가 낸 돈이 35000원이란건 좀 심하지 않니? 남자라고 해서 돈 많이 내야되는건 아니잖아?"<br><br>

<?php
	$man_share_for_cost_dinner = 15000;
?>
그러자 여자가 말하길, "헐, 나 오빠 진짜 그렇게 안봤는데... 나도 솔직히 할 말 많거든? 아까 파스타집에서 같이 먹자고 시킨 까르보나라 솔직히 오빠가 다 먹었잔아? 나도 점심 못 먹어서 배고픈데 오빠 생각해서 참은거거든? 오빠가 먹은 걸 생각해봐 좀. 25000원 중에 15000원도 넘겠다, 진짜. 그럼 오빠 식으로 계산하면 오빠가 밥값 <?php echo $man_share_for_cost_dinner; ?>원이랑 나머지에서 반땡한거 <?php echo $man_share_for_cost_movie + $man_share_for_cost_coffee; ?>원 내야되니까 35000원 딱이네? 안그래?" <br><br>

<?php
	$man_share_for_cost_popcorn = 4000;
	$man_share_for_cost_movie = $cost_ticket * 1 + $man_share_for_cost_popcorn;
	$man_share_for_cost_total = $man_share_for_cost_dinner + $man_share_for_cost_movie + $man_share_for_cost_coffee; // 다시계산 (반복)
?>
남자가 말하길, "와, 누가 들으면 내가 너 굶기고 다니는 줄 알겠다? 근데 너만 어이없는 거 아니거든? 아까 영화 볼 때 샀던 12000원짜리 팝콘은 누가 다 먹었을까? 그거생각하면 내가 내야 할 돈은 <?php echo $man_share_for_cost_total ?>원이 되는거야. 하지만 난 35000원을 냈지." <br><br>

<?php
	$cost_coffee = $cost_coffee + 5000;
	$man_share_for_cost_coffee = $cost_coffee * $man_ratio_for_dating_cost;
	$man_share_for_cost_total = $man_share_for_cost_dinner + $man_share_for_cost_movie + $man_share_for_cost_coffee; // 다시계산 (반복)
	$man_spent_money = $man_spent_money + 5000;
?>
"또 커피집에서 내가 가져온 쿠폰으로 정가 5000원짜리 치즈케익 먹은 건 왜 빼먹니? 그거까지 치면 내가 내야 할 돈은 <?php echo $man_share_for_cost_total ?>원인데, 내가 쓴 돈은 <?php echo $man_spent_money ?>원이 되는거야. 알겟냐?" <br><br>

<?php
	// 어떻게 해야 되지?
?>
여자가 말하길, "오빠 진짜 쪼잔하다. 그럼 오늘 계산한거 전부 다 오빠가 현금영수증 떼 갔으니까 연말정산때 소득공제 받을것도 다 계산해야겠네?"<br><br>
</body>
</html>