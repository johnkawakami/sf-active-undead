<?php
session_start();
include_once("tags.lib.php");

$id = filter_input( INPUT_GET, 'id', FILTER_VALIDATE_INT );
if (!$id) {
    http_response_code(400);
    echo "invalid id";
    include('tagfooter.php');
    exit;
}

$_SESSION['url'] = 'index.php?id='.$id;
session_write_close();

try {
    $db = new SFACTIVE\DB();
    $article = new SFACTIVE\Article($id);
} catch(Exception $e) {
    http_response_code(410);
    echo $e->getMessage();
    include('tagfooter.php');
    exit;
}

$display = $article->article['display'];
if ($display=='f') { 
    // only local stories get tags pages for now
    // this code can be modded to affect only hidden, for exampl3
    $db->execute_statement("DELETE FROM tags_articles WHERE article_id=$id");
    http_response_code(410);
    echo "Only local stories get a tag page";
    include('tagfooter.php');
    exit;
}

$subdirectory = $id % 100;
$cache_path = TAGS_CACHE_PATH."$subdirectory/$id";
if (file_exists($cache_path)) {
	$html = file_get_contents($cache_path);
	if (!preg_match('/MySQL server has gone away/m',$html)) {
			echo $html;
			exit;
	} else {
		unlink($cache_path);
	}
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
<meta name="robots" content="noindex" />
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
mkdir(TAGS_CACHE_PATH."$subdirectory");
file_put_contents( $cache_path, $text );
ob_end_flush();
echo "<p>fresh</p>";
