<?php
include_once("shared/global.cfg");

$id = $_GET['id'];
$type = $_GET['q'];
$format = $_GET['format'];
$callback = $_GET['callback'];

if (!filter_var($id,FILTER_VALIDATE_INT)) { exit; }

if ($format != 'json') {
	$format = 'html';
}

$ip = $_COOKIE['snitchip'];
if (!$ip) 
{
	$ip = abs(crc32($_SERVER['REMOTE_ADDR']));
	setcookie('snitchip',$ip, time()+60*60*24*2600);
}
else
{
	// renew the cookie
	setcookie('snitchip',$ip, time()+60*60*24*2600);
}

if ($format=='html') {
	?>
	<body style="margin: 0px"
	<p style="margin: 4px;">
	<strong>Peer Moderation</strong>
	<br />
	<br />
	<?
}

$db = new DB();
$out = array(); // for json output

if ( preg_match('/[0-9]+/',$id) &&
	preg_match('/(dmca|fraud|racist|genocide|chatter|porn|double|spam|ad|hate|offtopic|bestof|troll)/',$type) )
{
	if ( too_many_reports( 12, 15 ) )
	{
		if ($format == 'html') {
			print('Thanks, but, you can only submit 15 reports per 12 hour period.');
		}
		if ($format == 'json') {
			$out['message'] = "You can only submit 15 reports per 12 hour period.";
		}
	}
	else
	{
		$modScore = report( $type, $id );
		if ($format == 'html') {
			print('Thanks for helping us moderate.<br>');
		}
		if ($modScore > 0)
		{
			if ($format == 'html') {
				print("<small>ID: $ip <br>Your Moderator Score: $modScore.</small>");
			} 
			if ($format == 'json') {
				$out['moderatorScore'] = $modScore;
			}
		}
		if ( ($modScore > 20) and  preg_match('/(dmca|fraud|racist|genocide|chatter|porn|spam|ad|hate|offtopic)/',$type) )
		{
			delete_and_adjust( $id );
		}
	}
}
else
{
	if ($format == 'html') {
		print('Invalid.');
	} else {
		header('Status: 500 Error', 500);
		header('Content-Type: application/json');
		echo json_encode(array('status'=>'error'));
		exit;
	}
}

function too_many_reports( $interval_hours, $max_reports_per_interval )
{
	global $ip;
	$db = new DB();
	$a = $db->query("SELECT COUNT(*) as c FROM qc 
					WHERE ip='$ip' 
					AND	DATE_SUB(reportDate, INTERVAL $interval_hours HOUR) < 0");
	if ($a[0]['c']>$max_reports_per_interval)
		return true;
	else return false;
}

function delete_and_adjust( $id )
{
	global $ip;
	global $db;

	$db->execute_statement("UPDATE qcrank SET score=score-0.5 WHERE ip=$ip");

	$article_obj = new Article;
	$article_obj->update_article_status($id, 'f');

	$db->execute_statement("DELETE FROM qc WHERE id=$id");

}

/**
 * @returns int modScore the moderator's moderation score.
 */
function report( $type, $id )
{
	global $ip;

	$db = new DB();
	$a = $db->query("SELECT score FROM qcrank WHERE ip=$ip");
	$modScore = $a[0]['score'];
	if ($modScore==Null) $modScore=0;


	// This increases the 'ranking' score on the article
	// The change to the score depends on the mod score of the moderator.
	// For every 10 mod points, they get an extra +1 boost to the article's rating.
	// Moderators with a negative mod score don't get to mod up articles.
	if ($type=='bestof')
	{
		$db->execute_statement(
			"UPDATE webcast SET rating=0 WHERE id=$id AND rating IS NULL");
		if ($modScore > 0)
				$increment = log10($modScore) + 1;
		else
				$increment = 0;
		$db->execute_statement(
			"UPDATE webcast SET rating=rating+$increment WHERE id=$id AND rating<10");
		$db->execute_statement(
			"UPDATE webcast SET rating=10 WHERE AND rating>10");
		$article_obj = new Article;
		$article_obj->update_article_status($id, 't');
		return $modScore;
	}

    if ($type=='troll')
    {
        $db->execute_statement(
            "UPDATE webcast SET rating=-1 WHERE id=$id AND rating IS NULL");
		if (mysql_affected_rows($GLOBALS['db_conn']) >= 1)
		{
			$article_obj = new Article;
			$article_obj->update_article_status($id, 't');
		}
		// probably should do a render_everything, cache_to_disk 
		// sequence instead....  but there should be a 
	    // simpler update_article method... ugh.
        return $modScore;
    }


	$a = $db->query("SELECT COUNT(*) as c FROM qcok WHERE id=$id");

	// If the article is "ok" and they report it, then you drop their
	// moderator score by one
	if ($a[0]['c']>0) 
	{
		$db->execute_statement("INSERT INTO qcrank (ip,score,lastIgnoreDate) VALUES ($ip,-1,NOW()) ON DUPLICATE KEY UPDATE score=score-1,lastIgnoreDate=NOW()");
		$a = $db->query("SELECT score FROM qcrank WHERE ip=$ip");
		return $a[0]['score']; 
	}

	$db->execute_statement(
		sprintf("INSERT INTO qc (type,id,ip,reportDate)
			VALUES ('%s',%d,'%s',NOW())",
			addslashes($type), $id, $ip));
	$db->execute_statement("INSERT INTO qcrank (ip,score,lastIgnoreDate) VALUES ($ip,0,NOW()) ON DUPLICATE KEY UPDATE lastIgnoreDate=NOW()");

	return $modScore;

}

if ($format == 'html') {
	?>
	</p>
	<p>
	<img src='/qc/0.jpg'>
	</p>
	<script>
	function c() { window.close(); }
	window.setTimeout('c()',6000);
	</script>
	<?
} 
if ($format == 'json') {
	header('Status: 200 OK', 200);
	header('Content-Type: application/json');
	if ($callback) {
		echo $callback.'(';
	}
	echo json_encode($out);
	if ($callback) {
		echo ')';
	}
	exit;
}
