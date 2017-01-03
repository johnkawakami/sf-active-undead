<?php
//Fix the newswire caches

$display=true;
include('shared/global.cfg');

$q = " INSERT IGNORE INTO webcast_parent_l 
       SELECT * FROM webcast WHERE display='l'
	";
$db = new DB();
$a = $db->query($q);

function unhtmlentities($str)
{
	$str = preg_replace('/&quot;/',"'",$str);
	$str = preg_replace('/</','&lt;',$str);
	return $str;
}
?>
<? include("../admin_header.inc"); ?>
This page refreshes the local stories search cache.
<? include('../admin_footer.inc'); ?>
