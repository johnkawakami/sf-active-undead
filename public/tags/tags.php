<?php
include_once("shared/vendor/autoload.php");
include_once("shared/global.cfg");
session_start();

$id = $_GET['id'];
if (!preg_match('/^[0-9]{1,7}$/', $id)) die();

$db = new SFACTIVE\DB();

$tag = $db->query("SELECT name,`ignore`,synonym FROM tags WHERE id=$id");
$name = $tag[0]['name'];
$ignore = $tag[0]['ignore'];
$synonym = $tag[0]['synonym'];

// if the tag contains a cr or nl, it's an error and we should just
// delete the tag
// this is a real edge case
if (strpos($name,"\n")!==false or strpos($name,"\r")!==false or $name==NULL) {
    $db->execute("DELETE FROM tags WHERE id=$id");
    $db->execute("DELETE FROM tags_articles WHERE tag_id=$id");
    $url = $_SESSION['url'];
    if ($url) {
	    header("Location: $url");
	    exit;
    } else {
	http_response_code(410); // gone
	echo "Error: 410 Gone.<p>";
        exit;
    }
}


if ($ignore==1) {
	http_response_code(410); // gone
	echo "Error: 410 Gone.<p>";
	echo "ignored tag: $name";
    $db->execute("DELETE FROM tags_articles WHERE tag_id=$id");
	exit;
}
if ($synonym) {
	echo "synonym $synonym";
	// replace with lookup code
	$synonym = addslashes($synonym);
	$tags = $db->query("SELECT id FROM tags WHERE name='$synonym'");
	$id = $tags[0]['id'];
	header("Location: tags.php?id=$id", true, 301);
	exit;
}
fix_articles($id,$name);

//------- functions ------

function get_articles($id) {
  global $db;
  return $db->query("SELECT article_id AS id,heading,summary FROM tags_articles JOIN webcast ON article_id=webcast.id WHERE tag_id = $id");
}

function fix_articles($id,$name) {
	global $db;
	echo "fix articles $id, $name";
	if (!preg_match('/^[0-9]{1,7}$/', $id)) die("invalid id");
	if (!preg_match('/^[a-z0-9.# ]+$/i', $name)) die("invalid name".var_dump($name));
	$db->execute("UPDATE 
		(SELECT id FROM tags WHERE synonym='$name') AS a JOIN
		tags_articles AS b ON a.id=b.tag_id
		SET tag_id=$id");
}
?>
<html>
<head>
<title>Los Angeles Indymedia : tag : <?=$name?></title>
</head>
<body>
<div id="tagedit" onclick="document.getElementById('frm').style.display='block'">
		<H1><?=$name?></H1>
</div>
<form id="frm" style="display:none" method="POST" action="fixtag.php">
  <input type="hidden" name="id" value="<?=$id?>" />
  <input type="checkbox" name="ignore" value="ignore">ignore</input><br />
  synonym:<input type="text" name="synonym"><br />
  <input type="submit">
</form>
<?
	$art = get_articles($id); 
	foreach($art as $article) {
		echo "<p><b><a href='/display.php?id=$article[id]'>$article[heading]</a></b></a> (<a  href='index.php?id=$article[id]'>tags</a>)";
		echo "<blockquote>$article[summary]</blockquote>";
	}

?>
<?php include('tagfooter.php') ?>
</body>
</html>
