<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';
	
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

$article_obj = new \SFACTIVE\Article;
$article_obj->update_article_status($id, 'f');

print "$id was hidden";
print "<p><a href=/admin/qc>continue</a></p>";

?>
	<script>
		function refresh() { document.location.href='/admin/qc'; }
		setTimeout('refresh()',1000);
	</script>
<?

// clear out the reports (to keep table clean)
$db = new \SFACTIVE\DB();

$db->execute("DELETE FROM qc WHERE id=$id");

// upgrade this ip's rank
$db->execute("INSERT INTO qcrank (ip,score,lastIgnoreDate) VALUES ($ip,1,NOW()) ON DUPLICATE KEY UPDATE score=score+1,lastIgnoreDate=NOW()");


//-----------------------------------------
include('../admin_footer.inc');

?>
