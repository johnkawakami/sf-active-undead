<?php
include_once("shared/vendor/autoload.php");
include_once("shared/global.cfg");
include(SF_CLASS_PATH."/spamc_class.inc");
session_start();

$db = new SFACTIVE\DB();

$tags = $db->query("SELECT name, synonym FROM tags WHERE `synonym`<>''");

foreach($tags as $tag) {
  $synonym = $tag['synonym'];
  $name = $tag['name'];
  echo "$name : $synonym</br>\n";
}
