<?php
include_once("tags.lib.php");

$id = $_GET['id'];
if (!preg_match('/^[0-9]{1,7}$/', $id)) { 
	$id=0; 
}
$db = new DB();
$c = $db->query("select b.id FROM (select distinct article_id from tags_articles order by article_id ) a join webcast  b on a.article_id=b.id where b.display='t' OR b.display='g' LIMIT 0,500");
?>
<h1>Touching all non-local stories</h1>

<? 
    $o = '';
	foreach($c as $row) {
		$articleid = $row['id'];
		$o[] = " article_id=$articleid";
	}
	echo join(' or ',$o);
	$db->execute_statement("DELETE FROM tags_articles WHERE ".join(' or ',$o));
?>
