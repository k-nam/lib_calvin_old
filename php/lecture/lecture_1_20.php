<html>
<body>
<?php
	$file_name = $_SERVER["DOCUMENT_ROOT"]."/data/5k_ansi.txt";
	$number_of_total_lines = 30;
	echo $file_name;
	echo "<br>";
	echo "d:/dev/web/data/5k_ansi.txt";
	$handle = fopen($file_name, "r");
	for ($line_number = 1; $line_number <= $number_of_total_lines; $line_number += 1) {
		// fgets는 파일의 다음 한 줄을 읽어서 그 문자열을 리턴한다.
		// 만약 더 이상 읽을 라인이 없다면, 0을 리턴한다.
		$line = fgets($handle);
		if ($line == false) {
			break;
		}
		$word = "";
		$wordAndRank = explode("\t", $line);
		if (count($wordAndRank) === 2) {
			$word = $wordAndRank[0];
			$rank = intval($wordAndRank[1]);
		} else { // error
			echo "Error occured!<br>";
			break;
		}
		echo "단어: ".$word." 순위: ".$rank,"<br>";
		
	}
	fclose($handle);
?>
</body>
</html>