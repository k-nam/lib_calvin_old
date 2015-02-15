<%@ page language="java" contentType="text/html; charset=EUC-KR"
	pageEncoding="EUC-KR"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<%@ page import="java.util.*" import="db.Dao"%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<title>Subject recommendation result page</title>
</head>
<body>
	<%
		String recommendedSubject = (String) request
				.getAttribute("recommendedSubject");
		String age = (String) request.getParameter("age");
		String[] hobbies = (String[]) request.getAttribute("hobbies");

		out.print("So you are " + age + " years old...");
		out.print("<br/>We recommend you to study " + recommendedSubject
				+ " now!");
		if (hobbies != null) {
			for (String hobby : hobbies) {
				out.print("<br/>so you like: " + hobby);
			}
		}
		out.print("<br/>init param example: paramName was mapped to "
				+ request.getAttribute("paramName"));
		out.print("<br/>contextParamName: "
				+ getServletContext().getInitParameter("contextParamName"));
		out.print("<br/>dao object's entryName is: "
				+ ((Dao) request.getAttribute("dao")).getEntryName());
	%>
</body>
</html>