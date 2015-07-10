<!DOCTYPE html>
<html lang="en">
<head>
<title>Home</title>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<script src="https://code.jquery.com/jquery-1.10.2.js"></script>


<script>
	$(document).ready(function() {
		$('[data-toggle="tooltip"]').tooltip();
	});
</script>
<script>
	$(document).ready(function() {
		$(".dropdown-menu").on('click', 'li a', function() {
			$(this).parent().parent().siblings(".btn:first-child").html($(this).text() + ' <span class="caret"></span>');
			$(this).parent().parent().siblings(".btn:first-child").val($(this).text());
		});
		$("#navitem0").addClass("active");
	});
</script>
<script>
	$(document).ready(function() {
		$("#navitem0").addClass("active");
	});
</script>

</head>
<body>
	<div class="container">
		<%@ include file="/bootstrap/navbar.jsp"%>
		<div class="row">
			<div class="col-xs-0 col-lg-3"></div>
			<div class="col-xs-12 col-lg-6">
				<h1>
					<span data-toggle="tooltip" title="Hooray!"> My First Bootstrap Page</span>
				</h1>
				<p>This is some text.</p>
				<div class="row">
					<div class="col-xs-4" style="background-color: lavender;">A</div>
					<div class="col-xs-4" style="background-color: lavenderblush;">B</div>
					<div class="col-xs-4" style="background-color: lavender;">C</div>
				</div>
				<br>
				<div class="row">
					<div class="col-xs-4" style="background-color: lavender;">D</div>
					<div class="col-xs-8" style="background-color: lavenderblush;">E</div>
				</div>
				<br>
				<div class="btn-group btn-group-lg">
					<button type="button" class="btn btn-primary">Apple</button>
					<button type="button" class="btn btn-primary">Samsung</button>
					<button type="button" class="btn btn-primary">Sony</button>
				</div>

				<br>
				<div class="dropdown">
					<button class="btn button-lg btn-primary dropdown-toggle" type="button" data-toggle="dropdown">
						Dropdown Example <span class="caret"></span>
					</button>
					<ul class="dropdown-menu">
						<li><a href="#">HTML</a></li>
						<li><a href="#">CSS</a></li>
						<li><a href="#">JavaScript</a></li>
					</ul>
				</div>
				<br>
				<div class="form-group">
					<label for="sel1">Select list:</label> <select class="form-control" id="sel1">
						<option>1</option>
						<option>2</option>
						<option>3</option>
						<option>4</option>
					</select>
				</div>
			</div>
		</div>
	</div>
</body>
</html>