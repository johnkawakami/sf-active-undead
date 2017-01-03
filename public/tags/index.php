<?php
session_start();
include_once("tags.lib.php");

$id = $_GET['id'];
if (!preg_match('/^[0-9]{1,7}$/', $id)) die();

$_SESSION['url'] = 'index.php?id='.$id;
session_write_close();

$cache_path = TAGS_CACHE_PATH."tags$id";
if (file_exists($cache_path)) {
	echo file_get_contents($cache_path);
	exit;
}

$db = new DB();
$article = new Article($id);

$display = $article->article['display'];
if ($display=='f') { 
  // only local stories get tags pages for now
  // this code can be modded to affect only hidden, for exampl3
  $db->execute_statement("DELETE FROM tags_articles WHERE article_id=$id");
  include('tagfooter.php');
  exit;
}

$c = $db->query("SELECT COUNT(*) AS c FROM tags_articles WHERE article_id=$id");
if ($c[0]['c']==0) {
	build_tags_list($article);
}

ob_start();
?>
<html>
<head>
<title>LA Indymedia : tag web : <?=$article->article['heading'];?></title>
<link rel="stylesheet" href="main.css" />
</head>
<body>
<a href="/display.php?id=<?=$id?>"><h1><?=$article->article['heading']?></h1></a>

<? 
	$t = get_tags_for_article($id);
	$tags = array();
	foreach($t as $tag) {
		$name = str_replace(' ','&nbsp;',$tag['tag']);
		$tags[$tag['tag']] = "<a href='tags.php?id=$tag[id]'>$name</a>";
	}
	ksort($tags);
	echo(join(', ',$tags));
?>

</body>
</html>
<?
$text = ob_get_contents();
file_put_contents( $cache_path, $text );
ob_end_flush();
echo "<p>fresh</p>";
