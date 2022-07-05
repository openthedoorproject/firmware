<?php
include_once("conexaoBD.php");

$senha = $_POST["senhaDigitada"];
$numLab = $_POST["numeroLaboratorio"];
$data = $_POST["dataAcesso"];
$hora = $_POST["horaAcesso"];

$sqlConsultar = "select cod_autorizacao where Senha = " . $senha . " and Laboratorio = " . $numLab.";";
$sqlInserir = "insert into Registro_Acesso (cod_autorizacao, Data_acesso, Hora_acesso, Laboratorio) 
values(" . $codAutorizacao . ", " . $data . ", " . $hora . ", " . $numLab . ");";


$result = mysqli_query($conn, $sqlConsulta); 


if ($resut->num_rows > 0) { 
  $row = $result->fetch_assoc();
  $codAutorizacao = $row["cod_autorizacao"];

  $result = mysqli_query($conn, $sqlInserir); 

  //retorna para o sistema embarcado os dados para permitir acesso

}
