<!DOCTYPE html>
<html lang="en">
<head>
<title>Patient Chart</title>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<script src="https://code.jquery.com/jquery-1.10.2.js"></script>
<script src="//code.jquery.com/ui/1.11.4/jquery-ui.js"></script>
<script type="text/javascript" src="https://www.google.com/jsapi"></script>
<link rel="stylesheet" href="//code.jquery.com/ui/1.11.4/themes/smoothness/jquery-ui.css">



<script type="text/javascript">
	var dates = [];

	google.load("visualization", "1.1", {
		packages : [ "calendar" ]
	});
	google.setOnLoadCallback(drawChart);

	function drawChart() {
		var dataTable = new google.visualization.DataTable();
		dataTable.addColumn({
			type : 'date',
			id : 'Date'
		});
		dataTable.addColumn({
			type : 'number',
			id : 'Won/Loss'
		});
		/*
		dataTable.addRows([ [ new Date(2012, 3, 13), 37032 ], [ new Date(2012, 5, 14), 38024 ],
				[ new Date(2012, 3, 15), 38024 ], [ new Date(2012, 6, 16), 38108 ], [ new Date(2012, 3, 17), 38229 ],
				// Many rows omitted for brevity.
				[ new Date(2013, 6, 4), 38177 ], [ new Date(2013, 9, 5), 4 ], [ new Date(2013, 9, 12), 38210 ],
				[ new Date(2013, 5, 13), 38029 ], [ new Date(2013, 9, 19), 38823 ], [ new Date(2013, 9, 23), 38345 ],
				[ new Date(2013, 9, 24), 38436 ], [ new Date(2013, 9, 30), 38447 ] ]);*/
		dataTable.addRows(dates);
		var chart = new google.visualization.Calendar(document.getElementById('calendar_basic'));

		var options = {
			title : "Life chart",
			height : 350,
			calendar : {
				cellSize : 15
			},
		};

		chart.draw(dataTable, options);
	}
</script>
<script>
	$(function() {
		$.datepicker.formatDate("yy-mm-dd");
		$("#datepicker").datepicker();
	});
</script>
<script>
	$(document).ready(function() {
		$("#navitem4").addClass("active");
		$("#submitbutton").click(function() {
			//alert($("#sel1").val());
			//alert($("#datepicker").val());
			var date = $("#datepicker").datepicker("getDate");
			dates.push([ date, date.getDate() ]);
			alert(dates);
			drawChart();
			var dataObject = {
				date : date.getTime(), recordedTime : Date.now()
			};
			$.ajax({
				url : "/DailyChart?arg=" + JSON.stringify(dataObject),
				type : "POST",
				async : false,
				success : function(result) {

				},
				error : function(xhr) {
					alert("Error");
				},
				timeout : 1000
			});
		});
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
			<div class="col-lg-2">Please log in to use this service.</div>
			<div class="col-lg-5">
				<form class="form-horizontal">
					<div class="form-group">
						<label for="inputEmail3" class="col-sm-2 control-label">Email</label>
						<div class="col-sm-10">
							<input type="email" class="form-control" id="inputEmail3" placeholder="Email">
						</div>
					</div>
					<div class="form-group">
						<label for="inputPassword3" class="col-sm-2 control-label">Password</label>
						<div class="col-sm-10">
							<input type="password" class="form-control" id="inputPassword3" placeholder="Password">
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-offset-2 col-sm-10">
							<div class="checkbox">
								<label> <input type="checkbox"> Remember me
								</label>
							</div>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-offset-2 col-sm-10">
							<button type="submit" class="btn btn-default">Sign in</button>
						</div>
					</div>
				</form>
			</div>
		</div>
		<br>




		<div class="row">
			<div class="col-lg-1">
				<span></span>
			</div>
			<div class="col-lg-10">
				<div id="calendar_basic" style="width: 100%; height: 350px;"></div>
			</div>

		</div>

		<div class="row">
			<div class="col-lg-1">
				<span></span>
			</div>
			<div class="col-lg-3">
				<p>
					Date: <input type="text" id="datepicker">
				</p>
			</div>
			<div class="col-lg-8">

				<form class="form-inline">
					<label for="sel1">Today I felt:</label> <select class="form-control" id="sel1">
						<option>manic</option>
						<option>hypo-manic</option>
						<option selected="true">normal</option>
						<option>bad</option>
						<option>depressed</option>
					</select>
				</form>

				<br>

				<form class="form-inline">
					<label for="sel1">Today I took:</label> <select class="form-control" id="sel2">
						<option>all of</option>
						<option>some of</option>
						<option>none of</option>
					</select> <label>my medication.</label>
				</form>

				<br>

				<form>
					Today I excesied <input type="radio" name="exercise" value="all" />much <input type="radio" name="exercise"
						value="some" />a little<input type="radio" name="exercise" value="none" />not at all <br>
				</form>
				<br>
				<form>
					Today I ate <input type="radio" name="food" value="good" />happily<input type="radio" name="food" value="trouble" />
					with difficulty <input type="radio" name="food" value="vomit" />with difficulty and vomitted <br>
				</form>
				<br>
				<form>
					Last night I slept<input type="radio" name="sleep" value="good" />happily<input type="radio" name="sleep"
						value="trouble" />with difficulty <input type="radio" name="sleep" value="none" />not a moment<br>
				</form>
				<br>
				<form>
					For today, I want to say <input type="text" name="extra" />
				</form>
				<br>
				<form>
					I am being hospitalized now <input type="checkbox" name="hospitalized" value="true" />
				</form>
				<br>

				<button type="button" class="btn btn-success" id="submitbutton">Submit</button>

			</div>
		</div>
	</div>
</body>
</html>