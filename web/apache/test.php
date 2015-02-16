<html>
<body>

<form action="../result.php" method="post">
Name: <input type="text" name="name"><br>
E-mail: <input type="text" name="email"><br>
Website: <input type="text" name="website"><br>
Comment: <textarea name="comment" rows="5" cols="40"></textarea><br>
Gender:
<input type="radio" name="gender" value="female">Female
<input type="radio" name="gender" value="male">Male<br>
<input type="submit">
</form>

<?php
echo "Echoing".$_SERVER["PHP_SELF"]."<br>";
$servername = "localhost:3306";
$username = "root";
$password = "calvin1729";

// Create connection
$conn = new mysqli($servername, $username, $password);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
echo "Connected successfully";
$conn->close()
?>

</body>
</html>