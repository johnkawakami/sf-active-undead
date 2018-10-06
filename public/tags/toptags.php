<?php
include_once("shared/vendor/autoload.php");
include_once("shared/global.cfg");
# include(SF_CLASS_PATH."/article_class.inc");
session_start();

$_SESSION['url'] = 'toptags.php';

$db = new SFACTIVE\DB();

$o = '';
$tags = $db->query("SELECT b.name AS name,b.id AS id, a.ct AS ct, b.ignore AS ig, b.synonym AS syn FROM (SELECT tag_id,COUNT(tag_id) AS ct FROM tags_articles GROUP BY tag_id ORDER BY ct DESC LIMIT 500)  a JOIN tags b ON a.tag_id=b.id WHERE b.ignore=0 and b.synonym=''");
foreach($tags as $tag) {
	$id = $tag['id'];
	$name = $tag['name'];
	$count = $tag['ct'];
	$o .= "<a href='tags.php?id=$id'>$name ($count)</a>;  \n";
}
?>
<html>
<head><title>Los Angeles Indymedia : top tags</title>
<body>
<?=$o?>
<?include('tagfooter.php');?>
</body>
</html>
