<html>
<body>
<?php
	$cost_total = 60000;
	$man_ratio_for_dating_cost = 0.6;
	$money_to_pay = 0; // ���� � ������ �𸣴ϱ�
	$sex = "����";
	if ($sex == "����") {
		$money_to_pay = $cost_total * $man_ratio_for_dating_cost;
	}
?>
������ �� ����Ʈ ����� 6�����̾���. ����Ʈ ����� 60%�� ���ڰ� ���� ���� �����ϴٰ� ��������. ����� <?php echo $sex; ?>���, ���� ���� �� ���� <?php echo $money_to_pay; ?>���̴�.
</body>
</html>