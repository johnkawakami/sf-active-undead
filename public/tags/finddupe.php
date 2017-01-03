<?php
/*
 * This is part of a tool to help discover duplicate messages.
 *
 * Scans the hashes and finds duplicate candidates. Saves results
 * into the duplicates table.
 *
 */

session_start();
include_once("tags.lib.php");

try {
    $db = new PDO("mysql:host=".DB_HOSTNAME.";dbname=".DB_DATABASE.";charset=utf8", DB_USERNAME, DB_PASSWORD);
} catch(PDOException $e) {
	$errors[] = $e->getMessage();
	print $e->getMessage();
}
$stmt = $db->prepare("select content_hash from article_content_hash ORDER BY content_hash");
$stmt->execute();

$lasthash = '';
while($row = $stmt->fetch()) {
	$hash = $row['content_hash'];
	if ($hash == $lasthash) {
		echo $hash . '<br />';
		$st = $db->prepare("INSERT IGNORE INTO duplicates (content_hash) VALUES (?)"); 
		$st->execute(array($hash));
		$st->closeCursor();
	}
	$lasthash = $hash;
}

