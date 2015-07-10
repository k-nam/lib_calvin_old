<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Patient Chart</title>
<script src="https://code.jquery.com/jquery-1.10.2.js"></script>
<script>
	$(document).ready(function() {
		$("#navitem0").addClass("active");
	});
</script>
</head>
<body>
	<jsp:include page="/bootstrap/navbar.jsp" />
	<jsp:include page="/bootstrap/index.jsp" />
</body>
</html>