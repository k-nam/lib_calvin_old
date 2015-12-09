<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Untitled Document</title>

<script language="javascript" src="lib/jquery-1.11.2.js"></script>
<script language="javascript" src="lib/jquery-ui.js"></script>
<link rel="stylesheet" href="lib/jquery-ui.css" />

<script>
$(document).ready(function() {
	alert("준비됐어?");
	function getSuggestion(keyword) {	
		if (keyword == "") { 
			return ["abc"];
		} 
		var resultArray;
		$.ajax({ 
			url: "autocomplete.php?input=" + keyword + "&num=20",
			async: false,
			success: function(result) {
				resultArray = result.split(" ");
			},
			error: function(xhr) {
				alert("Error")
			},
			timeout : 1000
		});	
		return resultArray;
	}
	
	$("#dictionary_input").autocomplete({
		search: function(event, ui) {
			var suggestion = getSuggestion($("#dictionary_input").val());
			//alert("Suggestions: <" + suggestion + ">");
			$("#dictionary_input").autocomplete("option", "source", suggestion);	
			//$("#dictionary_input").autocomplete("option", "source", ["aaa", "abb", "abc", "bbb", "ccc"]);	
		},
		focus: function(event, ui) {
			// prevent autocomplete from updating the textbox
			event.preventDefault();
			// manually update the textbox
			$("#dictionary_input").val(ui.item.label);
		},
		select: function(event, ui) {
			// prevent autocomplete from updating the textbox
			event.preventDefault();
			// manually update the textbox and hidden field
			$(this).val(ui.item.label);
			$("#dictionary_input").val(ui.item.value + " go!");
		},
		delay: 10
	});	
});
</script>
</head>

<body>
<h1>남기웅이의 홈페이지에 온 걸 환영해!</h1><br><br>
<b>여기에 아무 영단어를 입력해봐 (자동완성 사전찾기)</b><br>
<input type="text" id="dictionary_input" class="textbox" size="20" /><br><br>
<a href = lecture/lecture_1_1.php>1_1</a><br><br>
아래는 참고만해! <br><br><br><br><br>
<form action="result.php" method="post">
이름: <input type="text" name="name"><br>
E-mail: <input type="text" name="email"><br>
Website: <input type="text" name="website"><br>
Comment: <textarea name="comment" rows="5" cols="40"></textarea><br>
성별:
<input type="radio" name="gender" value="female">Female
<input type="radio" name="gender" value="male">Male<br>
<input type="submit">
</form>

<?php
require 'mylib.php';

echo "Echoing".$_SERVER["PHP_SELF"]."<br>";
echo "PHP version is: ".phpversion()."\n";
//getMssqlConnection();
?>
<object data="sample.pdf" type="application/pdf"  width="800px" height="1200px">
	<embed src="sample.pdf">
</object>
<iframe src="web/viewer.html?file=/sample.pdf" width="800px" height="1200px"/>
</body>
</html>