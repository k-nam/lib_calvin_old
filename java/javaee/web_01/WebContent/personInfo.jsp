<%@ page language="java" contentType="text/html; charset=EUC-KR"
	pageEncoding="EUC-KR"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<%@ page import="java.util.*"%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<title>Insert title here</title>
</head>
<body>

	<h3>Me2day user finder!!!</h3>

	<form method="POST" action="Me2day.FindUser">
		<p>Enter user nickname you want to find!</p>
		<input type="text" name="nickname""></input><br /> <input
			type="SUBMIT" value="Find information for me" />
	</form>

	<%
	List<String> info = (List<String>)request.getAttribute("info");
	if (info == null) {
		out.print("how come null??");
		return;
	}
	out.print("<table border='1'");
	out.print("<tr>");
	out.print("<th>ID</th>");
	out.print("<th>Description</th>");
	out.print("</tr>");
	for (int i = 0; i <  info.size(); i++) {
		if (i % 2 == 0) {
			out.print("<tr>");
			out.print("<td>");
			out.print(info.get(i));
			out.print("</td>");
		} else {
			out.print("<td>");
			out.print(info.get(i));	
			out.print("</td>");
			out.print("</tr>");
		}		
	}	
%>

</body>
</html>