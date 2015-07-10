<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Practice</title>
<script src="https://code.jquery.com/jquery-1.10.2.js"></script>
<script>
	$(document).ready(function() {
		$("#navitem2").addClass("active");
	});
</script>
</head>
<body>
	<jsp:include page="/bootstrap/navbar.jsp" />
	<jsp:include page="/practice/index.jsp" />
</body>
</html>