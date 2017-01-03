<?php
$u = $_GET['u'];
if (!preg_match("/^\d+\/\d+\/.+$/",$u)) {
	echo 'bad filename '.$u;;
	exit;
}
$u = 'uploads/'.$u;


?>
<h1>Welcome to LA Indymedia</h1>
<p>Due to bandwidth costs, we're not allowing "hotlinking" of large files.</p>

<?=$u?>
