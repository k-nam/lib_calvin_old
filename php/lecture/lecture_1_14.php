<html>
<body>
<?php	
	function getTotalDataForSeptember($data_increase_per_day) { 
		$data_total = 0;
		$data_per_day = 100;
		for ($day = 1; $day <= 30; $day += 1) { 
			$data_total += $data_per_day;
			$data_per_day += $data_increase_per_day; 
		}
		return $data_total;
	}
	function getTotalDataForOctober($data_increase_per_day) { 
		$data_total = 0;
		$data_per_day = 100;
		for ($day = 1; $day <= 31; $day += 1) { // 30���� 31�� �ٲ� �� ���̴�
			$data_total += $data_per_day;
			$data_per_day += $data_increase_per_day; 
		}
		return $data_total;
	}
?>
�豺�� �� �� ���� �ϸ� ����Ʈ���� �տ��� ���� ���ϴ� �ߵ��ڴ�. ������ ����� �� �ִ� �����ͷ����� �Ѱ谡 �ֱ� ������, �Ŵ� �ʿ��� �׻� "�̹����� ��¥ ���� �����." �ϰ� ����� �Ѵ�. ������ �� ����� �׻� �������� ���Ƽ�, �Ϸ��Ϸ簡 �������� ���ݾ� ���ͳ��� �� ���� �ϰ� ����.<br><br>
�跮ȭ �� �� ���, �豺�� �Ŵ� ù������ 100�ް�����Ʈ�� ����ϰ�, �� ���ķδ� �Ϸ翡 10�ް�����Ʈ�� �� ���� ����Ѵٰ� ��������. �� �Ŵ� 2�Ͽ��� 110�ް�����Ʈ��, 3�Ͽ��� 120�ް�����Ʈ�� ����ϴ� ���̴�. <br><br>
�̹� ���� 6���̶�� �����ϸ�, �豺�� �̹� �� �� ���� ���Ե� �� �����ͷ��� ���ϱ�? �ݺ����� �Ἥ ���� ���غ���!<br><br>
��: <?php echo getTotalDataForSeptember(10); ?>�ް�����Ʈ<br><br>

����ö�� �����鼭 �豺�� ����Ʈ�� �ߵ��� ���� ��������, ���� �����ϴ� ������ ��뷮�� 10�ް�����Ʈ�� �ƴ� 15�ް�����Ʈ�� �Ǿ��ٰ� �Ѵ�. 9�� �Ѵ޵��� ���Ե� �� �����ͷ��� ���ϱ�?<br><br>
��: <?php echo getTotalDataForSeptember(15); ?>�ް�����Ʈ<br><br>

���� ��Ȳ���� �豺�� 10���� ���Ե� �� �����ͷ���?<br><br>
��: <?php echo getTotalDataForOctober(15); ?>�ް�����Ʈ<br><br>
</body>
</html>