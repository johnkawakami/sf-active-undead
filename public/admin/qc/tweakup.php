<?php
	
$display=true;
include('shared/global.cfg');
include("../admin_header.inc");

$id = $_GET['id'];
$ip = $_GET['ip'];
if (!filter_var($id,FILTER_VALIDATE_INT)) {
	exit;
}
if (!filter_var($ip,FILTER_VALIDATE_INT)) {
	exit;
}


print "tweaked the scores so people with mod scores below zero are now 1.0 points up.";
print "<p><a href=/admin/qc>continue</a></p>";

?>
	<script>
		function refresh() { document.location.href='/admin/qc'; }
		setTimeout('refresh()',1000);
	</script>
<?

// clear out the reports (to keep table clean)
$db = new DB();

// upgrade this ip's rank
$db->execute_statement("UPDATE qcrank SET score=score+1 WHERE score < 0");


//-----------------------------------------
include('../admin_footer.inc');

?>
