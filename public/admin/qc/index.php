<?php
//This page does bulk status changes

$display=true;
include('shared/global.cfg');

$q = "  SELECT qc.id AS id, 
        LEFT(webcast.article,800) AS excerpt, 
	    COUNT(qc.id) AS score, 
		webcast.parent_id AS parent_id, 
		qc.ip AS ip, 
		webcast.heading AS heading, 
		qcrank.score AS user_rep, 
		webcast.author AS author, 
		qc.type AS type
		FROM qc, webcast, qcrank
		WHERE qc.id = webcast.id
		AND qc.ip = qcrank.ip
		AND webcast.display<>'f'
		AND qc.type<>'bestof'
		AND reportDate > DATE_SUB(NOW(), INTERVAL 3 DAY)
		GROUP BY qc.id, qc.ip
		ORDER BY score ASC
	";
$db = new DB();
$a = $db->query($q);

$q = " SELECT ip,score AS user_rep FROM qcrank WHERE lastIgnoreDate > DATE_SUB(NOW(), INTERVAL 3 DAY) AND score < 0 ORDER BY score ASC";
$assholes = $db->query($q);

$q = " SELECT ip,score AS user_rep FROM qcrank WHERE lastIgnoreDate > DATE_SUB(NOW(), INTERVAL 7 DAY) AND score > 0 ORDER BY score DESC";
$heroes = $db->query($q);

function unhtmlentities($str)
{
	$str = preg_replace('/&quot;/',"'",$str);
	$str = preg_replace('/</','&lt;',$str);
	return $str;
}
?>
<? include("../admin_header.inc"); ?>
<b>Reported Posts (Quality Control)</b>
<p>These are reported posts, ordered by the number of reports.</p>
<p>The column on the right are IP codes for people who report posts that are okay.  Clicking on one of the links there will cause all that user's reports to be deleted.</p>
<div style="float:right; width:200px; padding:10px;">
	<b>Bottom moderators from the last 3 days.</b>
	<?
		foreach($assholes as $row)
		{
			print "<br><a href=ignoreall.php?ip=".$row['ip'].">ignore from ".$row['ip']." (".$row['user_rep'].")</a>";
		}
	?>
	<b>Top moderators from the last 7 days.</b>
	<?
		foreach($heroes as $row)
		{
			print "<br><a href=hideall.php?ip=".$row['ip'].">elevate ".$row['ip']." (".$row['user_rep'].")</a>";
		}
	?>
	<hr>
	<a href=tweakup.php>Tweak bad moderator scores up 1 points.</a><br /> 
</div>

<?
reset($a);
foreach($a as $row)
{
	if ($row['parent_id']==0) $row['parent_id']=$row['id']; // if it's a toplevel post, set the parent to itself
	print '<b>'.$row['id']."</b> : score ".$row['score']." ";
	print '('.$row['type'].'):';
	print "<a target='_blank' href=/display.php?id=".$row['id'].">context</a>  : ";
	print "<a href=hide.php?id=".$row['id']."&ip=".$row['ip'].">hide</a>";
	print " : <a href=ignore.php?id=".$row['id']."&ip=".$row['ip'].">ignore reports about ".$row['id']."</a>";
	print " : <a href=ignoreall.php?ip=".$row['ip'].">ignore all reports from ".$row['ip']." (".$row['user_rep'].")</a>";
	print "<br>";
	print 'title: <b>'.preg_replace('/\n/','<br>',unhtmlentities($row['heading'])).'</b><br>';
	print 'author: '. preg_replace('/\n/','<br>',unhtmlentities($row['author']));
	print ' <a href="../log/file_viewer.php?filename=ip_log.txt&str1='.urlencode(unhtmlentities($row['ip'])).'">find in ip log</a>';
	print '<br>';
	print preg_replace('/\n/','<br>',unhtmlentities($row['excerpt']));
	print "<hr>";
}
?>

<? include('../admin_footer.inc'); ?>
