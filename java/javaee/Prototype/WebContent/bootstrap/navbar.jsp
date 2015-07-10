<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta charset=UTF-8">
<title>Bootstrap navbar</title>
<script src="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>
<link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">
</head>
<body>
	<nav class="navbar navbar-default">
	<div class="container-fluid">
		<div class="row">
			<div class="col-xs-0 col-md-3"></div>
			<div class="col-xs-12 col-md-6">
				<div class="navbar-header">
					<div class="navbar-brand">WebSiteName</div>
				</div>
				<div>
					<ul class="nav navbar-nav">
						<li id="navitem0" class="navitem"><a href="/bootstrap/tab0.jsp">Home</a></li>
						<li id="navitem1" class="dropdown navitem"><a class="dropdown-toggle" data-toggle="dropdown" href="">Page
								1 <span class="caret"></span>
						</a>
							<ul class="dropdown-menu">
								<li id="navitem1_0" class="navitem"><a href="/bootstrap/tab1_1.jsp">Page 1-1</a></li>
								<li id="navitem1_1" class="navitem"><a href="/bootstrap/tab1_2.jsp">Page 1-2</a></li>
								<li id="navitem1_2" class="navitem"><a href="/bootstrap/tab1_3.jsp">Page 1-3</a></li>
							</ul></li>
						<li id="navitem2" class="navitem"><a href="/bootstrap/tab2.jsp">Practice</a></li>
						<li id="navitem3" class="navitem"><a href="/bootstrap/tab3.jsp">Blog</a></li>
						<li id="navitem4" class="navitem"><a href="/bootstrap/tab4.jsp">Patient Chart</a></li>
					</ul>
				</div>
			</div>
		</div>
	</div>
	</nav>
</body>
</html>