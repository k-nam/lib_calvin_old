<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>

<html>
<head>
<style type="text/css">
	body {
		background-color: #C2A7F2;
		font-family: sans-serif;		
	}
	h1 {
		color: #2A1959;
		border-bottom: 2px solid #2A1959;
	}
	h2 {
		color: #474B94;
		font-size: 1.2em;
	}
	h2, p {
		
	}
</style> 
</head>

<body>
<h1 align="center">Programming Study Menu</h1>
<h2>Subject selection</h2>
<h3>h333</h3>
<h4>h4</h4>
<h5>h5</h5>
<h6>h6</h6>
<p>This is the first paragraph.<br/>
<em>Select what you want!</em></p>
<form method="POST" action="Select.do">
	<p>Your age is:<input type="text" name="age"/></p>
	<p>So you want to study:<br/>
	<input type="checkbox" name="hobby" value="reading"/>reading<br/>
	<input type="checkbox" name="hobby" value="skiing"/>skiing<br/>
	<input type="checkbox" name="hobby" value="game"/>game</p>
	<blockquote><p>The more you study, the better.<br/>
		<em>by Calvin Caulfield</em></p></blockquote>
	<p><input type="SUBMIT" value="Choose my subjects for me!"/></p>
</form>

<hr/>
<h2>File download</h2>
<form method="GET" action="abc.jar">
	Click here to download text file
	<input type="SUBMIT" value="파일 다운로드"/>
</form>

<hr/>
<p><img src="files/aoi_yuu.jpg" width="50%" height="50%" alt="아오이 유우" />
<c:choose>
	<c:when test="${ sessionScope.isLoggedIn eq 'true' }">
		<jsp:include page="logOutBox.jsp" />
	</c:when>
	
	<c:otherwise>
		<jsp:include page="logInBox.jsp" />
	</c:otherwise>
</c:choose>
</p>

<p>ul example</p>
<ul>
	<li>Some defs
		<dl>
			<dt>term1</dt>
			<dd>define</dd>
			<dd>???</dd>
			<dt>term2</dt>
			<dt>term2</dt>
			<dd>define another</dd>
		</dl>
	</li>	
	<li> Ordered list
		<ol>
			<li>first</li>
			<li>second</li>
		</ol>
	</li>
</ul>	

<jsp:include page="personInfo.jsp" />

</body>
</html>


