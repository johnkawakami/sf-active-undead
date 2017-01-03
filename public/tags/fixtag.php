<?php
include_once("shared/global.cfg");
include(SF_CLASS_PATH."/spamc_class.inc");
# include(SF_CLASS_PATH."/article_class.inc");
include_once('tags.lib.php');
session_start();

$id = $_POST['id'];
if (!preg_match('/^[0-9]{1,7}$/', $id)) die();
$ignore = $_POST['ignore'];
if (!preg_match('/^(ignore|)$/', $ignore)) die();
$synonym = $_POST['synonym'];
if (!preg_match('/^[0-9a-z. ]*$/i', $synonym)) die();

if ($ignore=='ignore') {
	$ig = 1;
} else {
    $ig = 0;
}

$db = new DB();

//echo "UPDATE tags SET `ignore`=$ig,synonym='$synonym' FROM tags WHERE id=$id";
$db->execute_statement("UPDATE tags SET `ignore`=$ig,synonym='$synonym' WHERE id=$id");

if ($ig==1) {
		$db->execute_statement("DELETE FROM tags_articles WHERE tag_id=$id");
}
if ($synonym!='') {
	$tag = get_tag_by_name($synonym);
	$newid = $tag['id'];
	$db->execute_statement("UPDATE tags_articles SET tag_id=$newid WHERE tag_id=$id");
}

// right now, anyone can mess with the tags...

// crap logic 
if ($_SESSION['url']!='' && isset($_SESSION['url'])) {
	$url = $_SESSION['url'];
	$_SESSION['url']='';
	session_write_close();
}
header("Location: $url");
exit;
