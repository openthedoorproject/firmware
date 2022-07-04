<?php
include_once("conexaoBD.php");

//possiveis dados que eu imagino que serão usados
$senha;
$requisitante;
$numLab;
$data;
$hora;
$codigoTranca;

$result = $conn.msqli_query($sqlConsulta);// busca  senha digitada no banco de dados

if($resut == true){ // verifica se a senha digitada tem permissao
 $result = $conn.mysqli_query($sqlInserir); // se os dados baterem na consulta, resgistra no banco de dados a entrada do requisitante

 //retorna para o sistema embarcado os dados para permitir acesso

}


?>