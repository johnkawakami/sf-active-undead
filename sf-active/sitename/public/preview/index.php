<?php

if (strlen($_GET['preview']) < 1)
{
$_GET['preview'] = 12;
}
header("location: ../index.php?preview=".$preview);
?>
