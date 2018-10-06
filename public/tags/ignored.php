<?php
include_once("shared/vendor/autoload.php");
include_once("shared/global.cfg");
include(SF_CLASS_PATH."/spamc_class.inc");
# include(SF_CLASS_PATH."/article_class.inc");
session_start();

$db = new SFACTIVE\DB();

$tags = $db->query("SELECT name, id FROM tags WHERE `ignore`=1");

foreach($tags as $tag) {
  $id = $tag['id'];
  $name = $tag['name'];
  echo "<a href='tags.php?id=$id'>$name</a></br>\n";
}
