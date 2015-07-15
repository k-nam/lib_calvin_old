<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Untitled Document</title>

<script src="https://code.jquery.com/jquery-1.10.2.js"></script>
<script src="https://code.jquery.com/ui/1.11.4/jquery-ui.js"></script>
<link rel="stylesheet" href="https://code.jquery.com/ui/1.11.4/themes/vader/jquery-ui.css" />

<script>
	$(document).ready(function() {
		var isShowed = true;
		$("#button1").text("Hide");
		$("#button1").click(function() {
			if (isShowed == true) {
				$("#test2").hide();
				$("#button1").text("Show");
				isShowed = false;
			} else {
				$("#test2").show();
				$("#button1").text("Hide");
				isShowed = true;
			}
			$.ajax({
				url : "/Blogajax?keyword=" + $("#test3").val(),
				success : function(result) {
					alert("Sucess, result was: " + result);
					$("#test1").text(result);
				},
				error : function(xhr) {
					alert("Error")
				},
				async : true,
				timeout : 1000
			});
		});
		/*
		$("#dictionary_button").click(function() {
			$.ajax({
				url : "/DictionaryAutocomplete?input=" + $("#dictionary_input").val() + "&num=20",
				async : true,
				success : function(result) {
					alert("DictionaryAutocomplete AJAX success");
					$("#dictionary_result").text(result);
				},
				error : function(xhr) {
					alert("Error")
				},
				timeout : 1000
			});
		});*/
		function getSuggestion2(keyword) {
			if (keyword == "") {
				return [ "abc" ];
			}
			var resultArray = [];
			$.ajax({
				url : "/DictionaryAutocomplete?input=" + keyword + "&num=20",
				async : false,
				success : function(result) {
					$("#test2").text(result);
					JSON.parse(result).forEach(function(elem) {
						resultArray.push(elem);
					});
				},
				error : function(xhr) {
					alert("Error");
				},
				timeout : 1000
			});
			return resultArray;
			//return ["aaa", "bbb"];
		}
		$("#dictionary_input").autocomplete({
			search : function(event, ui) {
				$("#dictionary_input").autocomplete("option", "source", getSuggestion2($("#dictionary_input").val()));
			},
			focus : function(event, ui) {
				// prevent autocomplete from updating the textbox
				event.preventDefault();
				// manually update the textbox
				$("#dictionary_input").val(ui.item.label);
			},
			select : function(event, ui) {
				// prevent autocomplete from updating the textbox
				event.preventDefault();
				// manually update the textbox and hidden field
				$(this).val(ui.item.label);
				$("#dictionary_input").val(ui.item.value + " go!");
			},
			delay : 10
		});

		$("#button").mouseenter(function() {
			$(this).hide();
		});

	});
</script>
<script>
	$(document).ready(function() {
		$("#navitem3").addClass("active");
	});
</script>
</head>

<body>
	<%@ include file="/bootstrap/navbar.jsp"%>
	<div class="container">
		<div class="row">
			<div class="col-lg-1">
				<span></span>
			</div>
			<div class="col-lg-10">
				<div class="header_area">
					<div class="header">
						<div class="logoimage">
							<a href="#"><img src="" alt="Insert Logo Here" name="Insert_logo" width="180" height="90" id="Insert_logo"
								style="display: block;" /></a>
						</div>
						<div class="menubar">
							<ul class="nav">
								<li><a href="#">Link one</a></li>
								<li><a href="#">Link two</a></li>
								<li><a href="#">Link three</a></li>
								<li><a href="#">Link four</a></li>
							</ul>
						</div>
					</div>
				</div>
				<div class="header_area2">
					<div class="header2">
						<nav>
							<ul>
								<li><a href="#">Home</a></li>
								<li><a href="#">Tutorials</a>
									<ul>
										<li><a href="#">Photoshop</a></li>
										<li><a href="#">Illustrator</a></li>
										<li><a href="#">Web Design</a>
											<ul>
												<li><a href="#">HTML</a></li>
												<li><a href="#">CSS</a></li>
											</ul></li>
									</ul></li>
								<li><a href="#">Articles</a>
									<ul>
										<li><a href="#">Web Design</a></li>
										<li><a href="#">User Experience</a></li>
									</ul></li>
								<li><a href="#">Inspiration</a></li>
							</ul>
						</nav>
					</div>
				</div>
				<div class="content">
					<p id="test1">This is text.</p>
					<p id="test2">This is paragraph.</p>
					<textarea id="test3">This is where the user can enter text...</textarea>
					<button id="button1">This is button</button>
					<p>
						Find: <input id="dictionary_input" />
						<button id="dictionary_button">Dictionary autocomplete</button>
					</p>
					<p>Autocomplete result</p>
					<p id="dictionary_result"></p>
					<!-- end .content -->
				</div>
				<div class="footer">
					<p>This .footer contains the declaration position:relative; to give Internet Explorer 6 hasLayout for the
						.footer and cause it to clear correctly. If you're not required to support IE6, you may remove it.</p>
					<!-- end .footer -->
				</div>
				<!-- end .container -->
			</div>
		</div>
	</div>
</body>
</html>