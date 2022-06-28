<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "clovisao";

$conn = mysqli_connect($servername, $username, $password, $dbname);

if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}
$nome = $_POST["innome"];
$cpf = $_POST["incpf"];
$idade = $_POST["inidade"];

$sql = "insert into gentes(nome, cpf,idade) values('$nome', '$cpf', $idade)";

if (mysqli_query($conn, $sql)) {
    echo "cadastrado com sucesso";
} else {
    echo "Erro: " . mysqli_error($conn);
}
