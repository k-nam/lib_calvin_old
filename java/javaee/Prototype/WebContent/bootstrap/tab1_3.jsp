<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Patient Chart</title>
<script src="https://code.jquery.com/jquery-1.10.2.js"></script>
<script>
	$(document).ready(function() {
		$("#navitem1_2").addClass("active");
	});
</script>
</head>
<body>
	<div class="container">
		<jsp:include page="/bootstrap/navbar.jsp" />
	</div>
</body>
</html>