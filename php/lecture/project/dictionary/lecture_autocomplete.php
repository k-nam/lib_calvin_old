<html>
<head>
<script language="javascript" src="/lib/jquery-1.11.2.js"></script>
<script language="javascript" src="/lib/jquery-ui.js"></script>
<link rel="stylesheet" href="/lib/jquery-ui.css" />

<script>
$(document).ready(function() {
	//alert("준비됐어?");
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
<?php
echo $_SERVER["DOCUMENT_ROOT"]."/data/5k_ansi.txt";
?>
<body>
<b>여기에 아무 영단어를 입력해봐 (자동완성 사전찾기)</b><br>
<input type="text" id="dictionary_input" class="textbox" size="20" /><br><br>
</body>
</html>