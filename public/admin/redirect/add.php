<?php // vim:ai:et:sw=4:ts=4 
include 'shared/vendor/autoload.php';

include('init.php');

$id  = filter_input( INPUT_POST, 'id', FILTER_SANITIZE_NUMBER_INT );
$url = filter_input( INPUT_POST, 'url', FILTER_SANITIZE_URL );

try {
	// $article = new \SFACTIVE\Article($id);
	
	$stmt = $pdo->prepare('insert into redirect (`id`,`url`) VALUES (:id, :url)');
	$stmt->execute([':id'=>$id, ':url'=>$url]);
} catch(Exception $e) {
	$_SESSION['error'] = $e->getMessage();
	echo $e->getMessage();
}

header('Location: index.php');
