<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="common" uri="RandomFunction"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<!DOCTYPE html>
<html>
<head>
<meta charset=UTF-8">
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