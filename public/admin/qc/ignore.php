<?php
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


$db = new SFACTIVE\DB();

$db->execute("INSERT INTO qcrank (ip,score,lastIgnoreDate) VALUES ($ip,-1,NOW()) ON DUPLICATE KEY UPDATE score=score-1,lastIgnoreDate=NOW()");
$db->execute("DELETE FROM qc WHERE id=$id");
$db->execute("INSERT INTO qcok (id) VALUES ($id)");

print "reports for $id are now ignored";

print "<p><a href=/admin/qc>continue</a></p>";
?>
    <script>
        function refresh() { document.location.href='/admin/qc'; }
        setTimeout('refresh()',1000);
    </script>
<?


//-----------------------------------------
include('../admin_footer.inc');

