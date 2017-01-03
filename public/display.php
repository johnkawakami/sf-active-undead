<?php
//display.php is used for displaying a single
//article from the DB

include("shared/global.cfg");

if ($_GET['article_id']) {
	# handles the old display.php3 params
    $id = $_GET['article_id'];
} else {
	$id = $_GET['id'];
}
$id = intval($id);

if ($article_id) $id = $article_id;
if ($id) { 
    $db_obj = new DB;
    $query = "SELECT created, parent_id FROM webcast WHERE id=".$id;
    $result = $db_obj->query($query);
    $article_fields = array_pop($result);
    $ts_date = $article_fields['created'];
	$parent_id = $article_fields['parent_id'];
    $Month = substr($ts_date,5,2);
    $Year = substr($ts_date,0,4);
    $pathtofile = "$Year/$Month/";
	if ($parent_id > 0)
	{
			$query = "SELECT created FROM webcast WHERE id=".$parent_id;
			$result = $db_obj->query($query);
			$article_fields = array_pop($result);
			$ts_date = $article_fields['created'];
			$Month = substr($ts_date,5,2);
			$Year = substr($ts_date,0,4);
			$pathtofile = "$Year/$Month/";
			$articlelink = $GLOBALS['news_url'] . $pathtofile . $parent_id . "_comment.php#". $id; 
	} else {
			$articlelink = $GLOBALS['news_url'] . $pathtofile . $id . ".php"; 
	}
    header("Location: $articlelink"); 
} else { 
    header("Location: /news/"); 
} 
?>
