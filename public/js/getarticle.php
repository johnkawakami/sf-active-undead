<?php
include('shared/vendor/autoload.php');
// based on display.php
// This redirects the user to the JS version of the page.

include("shared/global.cfg");

$id = $_GET['id'];
$id = intval($id); // it must be an integer
if (!$id) { // id must be present
  echo "blank parameter";
	exit;
}

$db_obj = new SFACTIVE\DB;
$query = "SELECT created FROM webcast WHERE id=".$id;
$result = $db_obj->query($query);
$article = array_pop($result);
$ts_date = $article['created'];
$month = substr($ts_date,5,2);
$year = substr($ts_date,0,4);
$pathtofile = "$year/$month/";
if ($article_fields['parent_id'] > 0) {
  echo "not a parent article";
	exit; // bail out if it's not a parent article
}
$link = $GLOBALS['news_url'].$pathtofile.$id.'.json';

header("Location: http://la.indymedia.org/js/?v=cont&url=$link", TRUE, 301);
// header("Location: $link");
exit();

