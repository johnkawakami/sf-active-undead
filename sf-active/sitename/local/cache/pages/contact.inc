<?php
$calendar_url = $GLOBALS['calendar_url'];
if ($_GET['refresh'])
{ 
	$table = '<table cellspacing="0" cellpadding="0" border="0"><tr><td>meeting<hr></td><td></td><td>date, time<hr></td><td></td><td>location<hr></td></tr>'; 
	$db_obj = new DB;
	$meetquery="select * from event where (title LIKE '%IMC%' OR title LIKE '%indymedia%') AND (TO_DAYS(start_date) - TO_DAYS(NOW()) >= 0) order by start_date limit 10"; 
	$resultset = $db_obj->query($meetquery);
	while ($next = array_pop($resultset))  
	{
		$event_id = $next[event_id]; 
		$title = $next[title]; 
		$start_date = $next[start_date]; 
		$location_details = $next[location_details];
		$table .= "<tr valign=\"top\"><td><code><a href=\"$calendar_url/event_display_detail.php?event_id=$event_id\">$title</a></code></td><td><code>&nbsp;</code></td><td><code>$start_date</code></td><td><code>&nbsp;</code></td><td><code>$location_details</code></td></tr><tr><td colspan=\"5\"><img src=\"/im/bump.gif\" height=\"4\" width=\"1\" border=\"0\" alt=\"\"></td></tr>";
	}
	$table .= '</table>';
	$fffp = fopen(SF_CACHE_PATH.'/contact.html',w); 
	fwrite($fffp, $table, strlen($table)); 
	fclose($fffp); 
}
?>

<br />

<h3 class="h3">Contact</h3>

<p>Any feedback with regards to site content should be directed to <a href="mailto:<?=$GLOBALS['public_email']?>"><?=$GLOBALS['public_email']?></a>. Do not email news releases or articles to the IMC - you must <a href="/publish.php">publish</a> stories yourself. Report technical problems to <a href="mailto:<?=$GLOBALS['public_email']?>"><?=$GLOBALS['public_email']?></a>. Other inquiries can be directed to <a href="mailto:<?=$GLOBALS['public_email']?>"><?=$GLOBALS['public_email']?></a>.

<h3 class="h3">Get Connected</h3>

<p>We are always looking for people to get involved as media producers and in other logistical roles. Please introduce yourself at our <a href="http://volunteer.indymedia.org/">volunteer signup page</a> and subscribe to one or more of our <a href="http://lists.indymedia.org/>email lists</a> (all SF Bay Area lists have the imc-sf prefix; click on a list to subscribe or view the archives). If you want to get involved with a specific working group, check out the <a href="#volunteer_by_group">volunteer contact list</a> below.

<!-- Meetings -->
<a name="meetings"></a>
<h3 class="h3">Upcoming Meetings</h3> 
<p>To receive weekly meeting invitations, subscribe to <a href="http://lists.indymedia.org/mailman/listinfo//">imc-xx-announce</a>.

<!-- You can add your Local meeting to the Calendar and it will show up here too -->
<p>
<?php include(SF_CACHE_PATH.'/contact.html'); ?>

<p align="right">
	<small><a href="contact.php?refresh=please_do">REFRESH</a> meeting times 
	[<?php echo(date("r",filemtime(SF_CACHE_PATH.'/contact.html'))); ?>]
	</small>
</p>

<!-- Space -->
<h3 class="h3">Visit the IMC Space</h3>
<p>Enter your IMC Space Loaction here.</a></p>

<!-- Groups -->
<a name="volunteer_by_group"></a>
<h3 class="h3">Working Group Volunteer Contacts</h3>

<p>There are already a number of groups that are working together around certain areas of interest. Why wait for a personal invitation?  If you want to get involved, please subscribe to the relevant email list below and send a message introducing yourself! But don't feel like you have to contact somebody before you <a href="/publish.php">publish</a> something yourself.</p>

<dl>
<!-- Template -->
<dt><b>Topic</b></dt> <dd><em>Email list:</em> <a
href="mailto: liste (at) indymedia.org">liste (at) indymedia.org</a>
 <br /> <em>To subscribe or view archived messages:</em> <a
href="http://lists.indymedia.org/mailman/listinfo/liste/">lists.indymedia.org/mailman/listinfo/liste</a>, 
<br />

...Description here...

</dd>

</dl>
