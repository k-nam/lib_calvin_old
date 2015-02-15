<%@ page language="java" contentType="text/html; charset=EUC-KR"
	pageEncoding="EUC-KR"%>
<%@ taglib prefix="common" uri="RandomFunction"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<title>Insert title here</title>
</head>
<body>

	<form method="POST" action="LogOut.do">
		<input type="SUBMIT" value="로그아웃" />
	</form>

	Welcome: ${user.id} ${user.name}
	<br /> test: ${ common:getRandom() }
	<br />
	<c:out value='<b>saa</b>' />
	<br />
	<c:out value='${user.name}' />
	<br />
	<c:out value='${user2}' default='null' />
	<br />
	<h3>
		<b>Hobby table of this user</b>
	</h3>

	<table>
		<c:forEach var="hobby" items="${user.hobbies}">
			<tr>
				<td><i>${hobby}</i></td>
			</tr>
		</c:forEach>
	</table>

</body>
</html>