<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>

<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>

<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<script src="https://code.jquery.com/jquery-1.10.2.js"></script>
<script>
	$(document).ready(function() {
		$("#navitem2").addClass("active");
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
			<div class="col-lg-10">
				<h1 align="center">Programming Study Menu</h1>

				<h2>Subject selection</h2>
				<h3>h333</h3>
				<h4>h4</h4>
				<h5>h5</h5>
				<h6>h6</h6>
				<p>
					This is the first paragraph.<br /> <em>Select what you want!</em>
				</p>
				<form method="POST" action="/Select.do">
					<p>
						Your age is:<input type="text" name="age" />
					</p>
					<p>
						So you want to study:<br /> <input type="checkbox" name="hobby" value="reading" />reading<br /> <input
							type="checkbox" name="hobby" value="skiing" />skiing<br /> <input type="checkbox" name="hobby" value="game" />game
					</p>
					<blockquote>
						<p>
							The more you study, the better.<br /> <em>by Calvin Caulfield</em>
						</p>
					</blockquote>
					<p>
						<input type="SUBMIT" value="Choose my subjects for me!" />
					</p>
				</form>

				<hr />
				<h2>File download</h2>
				<form method="GET" action="abc.jar">
					Click here to download text file <input type="SUBMIT" value="파일 다운로드" />
				</form>

				<hr />
				<p>
					<img src="/files/ebisu.jpg" width="50%" height="50%" alt="Ebisu Garden Place" />
					<c:choose>
						<c:when test="${ sessionScope.isLoggedIn eq 'true' }">
							<jsp:include page="logOutBox.jsp" />
						</c:when>

						<c:otherwise>
							<jsp:include page="logInBox.jsp" />
						</c:otherwise>
					</c:choose>
				</p>

				<button type="button" onclick="document.getElementById('demo').innerHTML = Date()">Click me to display Date
					and Time.</button>

				<p id="demo"></p>

				<p>ul example</p>
				<dl>
					<dt>Unordered list
					<dd>
						<ul>
							<li>First</li>
							<li>Second</li>
							<li>Third</li>
						</ul>
					<dt>Ordered list
					<dd>
						<ol>
							<li>First</li>
							<li>Second</li>
							<li>Third</li>
						</ol>
				</dl>

			</div>
		</div>
	</div>
</body>
</html>


