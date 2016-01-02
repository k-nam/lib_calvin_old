<html>
<body>
<?php		
	$string1 = "월화수";
	$string2 = "목금토";
	$string3 = "나는 주말이 좋아";
	$string4 = "I like weekends";
	
	echo "문자열 합친 결과 = ".$string1.$string2;
	echo "<br>";
	
	echo $string1."의 길이 = ".strlen($string1);
	echo "<br>";
	
	// 한 개의 글자로 된 문자열 '"' 을 이해해 보자
	echo '"'.$string1.'"'."의 길이 = ".strlen($string1);
	echo "<br>";
	
	echo '"'.$string1.'"'."의 한글 글자로 측정한 길이 = ".mb_strlen($string1, "utf8");
	echo "<br>";
	
	echo '"'.$string3.'"'."의 한글 글자로 측정한 길이 = ".mb_strlen($string3, "utf8");
	echo "<br>";
	
	echo '"'.$string3.'"'."의 단어 갯수 = ".str_word_count($string3);
	echo "<br>";
	
	echo '"'.$string4.'"'."의 단어 갯수 = ".str_word_count($string4);
	echo "<br>";
	
	echo '"'.$string3.'"'."의 뒤집기 = ".strrev($string3);
	echo "<br>";
	
	echo '"'.$string4.'"'."의 뒤집기 = ".strrev($string4);
	echo "<br>";	
?>

</body>
</html>