<?php
//display.php is used for displaying a single
//article from the DB

include("shared/global.cfg");
if ($article_id) $id = $article_id;
if ($id) { 
    $db_obj = new DB;
    $query = "select created from webcast where id=".$id;
    $result = $db_obj->query($query);
    $article_fields = array_pop($result);
    $ts_date = $article_fields[created];
    $Month = substr($ts_date,4,2);
    $Year = substr($ts_date,0,4);
    $pathtofile = "$Year/$Month/";
    $articlelink = $GLOBALS['news_url'] . $pathtofile . $id . ".php"; 
    header("Location: $articlelink"); 
} else { 
    header("Location: /news/"); 
} 
?>
