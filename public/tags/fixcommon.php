<?php
include_once("shared/global.cfg");
include(SF_CLASS_PATH."/spamc_class.inc");
# include(SF_CLASS_PATH."/article_class.inc");

$db = new DB();

$ta = new TextAnalyzer();

foreach($ta->common as $word=>$v) {
	$tag = $db->query("SELECT id FROM tags WHERE name='".$db->quote($word).'");
	$id = $tag[0]['id'];
	if ($id>0) {
		echo "<p>deleting $word, $id</p>";
		$db->execute_statement("DELETE FROM tags_articles WHERE tag_id=$id");
		$db->execute_statement("DELETE FROM tags WHERE id=$id");
	}
}

