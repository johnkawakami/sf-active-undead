<?php
namespace SFACTIVE;

include('shared/vendor/autoload.php');
include_once("tags.lib.php");


$id = $_GET['id'];
if (!preg_match('/^[0-9]{1,7}$/', $id)) { 
	$id=0; 
}
$db = new DB();
$c = $db->query("SELECT id FROM webcast WHERE parent_id=0 AND id>$id ORDER BY id ASC  LIMIT 0,100");
?>
<h1>Rebuilding tags indexes</h1>

<? 
	$last = $c[0]['id'];
	foreach($c as $row) {
		$article = new Article($row['id']);
		echo "$row[id]<br>\n";
		build_tags_list($article);
		$article = NULL;
	}
	echo "<p><a href='rebuild.php?id=$last'>continue</a></p>";
?>
