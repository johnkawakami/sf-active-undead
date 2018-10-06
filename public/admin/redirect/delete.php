<?php // vim:ai:et:sw=4:ts=4 // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

include('init.php');

$id  = filter_input( INPUT_POST, 'id', FILTER_SANITIZE_NUMBER_INT );

try {
	$stmt = $pdo->prepare('delete from redirect where id=:id');
	$stmt->execute([':id'=>$id]);
} catch(Exception $e) {
	$_SESSION['error'] = $e->getMessage();
}

header('Location: index.php');
