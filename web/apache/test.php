<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Untitled Document</title>

<script language="javascript" src="lib/jquery-1.11.2.js"></script>
<script language="javascript" src="lib/jquery-ui.js"></script>
<link rel="stylesheet" href="lib/jquery-ui.css" />

<script>
$(document).ready(function() {
	alert("ready");
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
		dealy: 10
	});	
});
</script>
</head>

<body>

<form action="result.php" method="post">
Name: <input type="text" name="name"><br>
E-mail: <input type="text" name="email"><br>
Website: <input type="text" name="website"><br>
Comment: <textarea name="comment" rows="5" cols="40"></textarea><br>
Gender:
<input type="radio" name="gender" value="female">Female
<input type="radio" name="gender" value="male">Male<br>
<input type="submit">
</form>

<input type="text" id="dictionary_input" class="textbox" size="20" />
<br>

<?php
echo "Echoing".$_SERVER["PHP_SELF"]."<br>";
$servername = "localhost:3306";
$username = "root";
$password = "calvin1729";

// Create connection
$conn = new mysqli($servername, $username, $password);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
echo "Connected successfully";
$conn->close();
?>

</body>
</html>