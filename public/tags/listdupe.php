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
$stmt = $db->prepare("SELECT content_hash FROM duplicates WHERE resolved=0 LIMIT 50");
$stmt->execute();

echo "<h1>List of duplicated hashes</h1>";
echo "<p>Click on a hash to see all the articles that match it.</p>";
while($row = $stmt->fetch()) {
	$hash = $row['content_hash'];
	echo "<a href='hashdetail.php?id=".$hash."'>$hash</a><br />\n";
}

?>
<a href="finddupe.php">Detect Dupes</a>
