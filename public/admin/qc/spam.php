<?php
//This page does bulk status changes

$display=true;
include('shared/global.cfg');

if (isset($_GET['end'])) {
	$end = $_GET['end'];
	if (!filter_var($end, FILTER_VALIDATE_INT)) {
		echo "bad end";
		exit;
	}
} else {
	$end = false;
}

$_SESSION['delete'] = 'user can delete';


if ($end===false) {
	$where = "";
} else {
	$where = "AND (webcast.id < $end) ";
}

$q = " SELECT id, heading, author, article
		FROM webcast
		WHERE 
		webcast.display='f'
		$where
		ORDER BY created DESC
		LIMIT 0,200
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
<b>Hidden Posts (Quality Control)</b>
<p>These are hidden posts, ordered by the date.</p>
<p>You may use this interface to delete junk posts.  These will be DELETED, not merely hidden.  Linked files will not be deleted, however.  This will save space, and also allow for better rendering on the newswire.</p>
<?
reset($a);
$lastid = 0;
foreach($a as $row)
{
	print '<b>'.$row['id']."</b>";
	print "<a href=delete.php?id=".$row['id'].">delete</a>";
	print "<br>";
	print 'title: <b>'.preg_replace('/\n/','<br>',unhtmlentities($row['heading'])).'</b><br>';
	print 'author: '. preg_replace('/\n/','<br>',unhtmlentities($row['author'])).'<br>';
	print substr(preg_replace('/\n/','<br>',unhtmlentities($row['article'])),0,200);
	print "<hr>";
	if ($lastid===0) { 
		$lastid = $row['id']; 
	}
}
echo "<p><p><a href='spam.php?end=$lastid'>previous</a></p>";
?>
<? include('../admin_footer.inc'); ?>
