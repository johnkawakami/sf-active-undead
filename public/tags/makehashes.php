<?php
/*
 * This is part of a tool to help discover duplicate messages.
 *
 * This script creates a table woth "content hashes" for each of the parent
 * articles. The content hash is based on the first 20 keywords extracted
 * by the SpamC class.
 * 
 * It runs in chunks. When there are no articles left to hash, it tries
 * to load new article_ids into the article_content_hash table, but this
 * probably fails.
 */
session_start();
include_once("tags.lib.php");

$db = new DB();
$c = $db->query("select article_id from article_content_hash where content_hash is null LIMIT 50");

if (count($c)==0) {
        echo "Loading the article_content_hash table.\n";
        echo "<p>Give it ten minutes and reload.</p>";
        ob_flush_clean();
        $db->query("INSERT IGNORE INTO article_content_hash (article_id) SELECT
 id FROM webcast WHERE parent_id=0 AND id > 260000");
        exit();
}


ob_start();
?>
<html>
<head>
<title>LA Indymedia : tag web : <?=$article->article['heading'];?></title>
<meta http-equiv="refresh" content="2">
<link rel="stylesheet" href="main.css" />
</head>
<body>
<pre>
<? 
	foreach($c as $row) {
		$id = $row['article_id'];
		$article = new Article($id);
		$title = $article->article['heading'];
		$text = $article->article['heading']. '  ';
		$text .= $article->article['summary'] . '  ';
		$text .= $article->article['article'];
		$image = $article->article['linked_file'];
		$s = new SpamC(strip_tags($text));
		$keywords = array_slice($s->keyword_counts, 0, 20);
		$hash = md5(implode(array_keys($keywords)).$image);
		if ($hash==$lasthash) { print "<font color=red>"; }
		print "$id : $hash : $title\n";
		if ($hash==$lasthash) { print "</font>"; }
		$lasthash = $hash;
		$db->query("UPDATE article_content_hash SET content_hash='$hash' WHERE article_id=$id");
	}
?>
</body>
</html>
<?
$text = ob_get_contents();
ob_end_clean();
echo $text;
