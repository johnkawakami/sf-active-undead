<?php

include 'shared/vendor/autoload.php';	

$display=true;
include('shared/global.cfg');
include("../admin_header.inc");

$ip = $_GET['ip'];

if (!filter_var($ip,FILTER_VALIDATE_INT)) {
	exit;
}
 
$db = new SFACTIVE\DB();

// first, find all the 'bestofs' by the yahoo, and demote those articles by ten
$db->execute("UPDATE qc,webcast SET webcast.rating=webcast.rating-10 WHERE qc.ip='$ip' AND qc.id=webcast.id AND qc.type='bestof'");

$db->execute("DELETE FROM qc WHERE ip='$ip'");

$db->execute("INSERT INTO qcrank (ip,score,lastIgnoreDate) VALUES ($ip,-5,NOW()) ON DUPLICATE KEY UPDATE score=score-5,lastIgnoreDate=NOW()");

print "reports for $id are now deleted";
print "and this IP hashcode was stored in the 'jerk' list.";

print "<p><a href=/admin/qc>continue</a></p>";

//-----------------------------------------
include('../admin_footer.inc');

