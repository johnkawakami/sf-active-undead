<?php
include 'shared/vendor/autoload.php';
	
$display=true;
include('shared/global.cfg');
include("../admin_header.inc");

$id = $_GET['id'];
if (!filter_var($id,FILTER_VALIDATE_INT)) {
	exit;
}

if (!preg_match('/^[0-9]+$/', $id)) die('invalid id');

if (strpos($_SERVER['HTTP_REFERER'],'http://la.indymedia.org/admin/qc/spam.php')===false) die('bad referrer');

if ($_SESSION['delete'] != 'user can delete') die('bad session');

$db = new SFACTIVE\DB();

$db->execute("DELETE LOW_PRIORITY FROM webcast WHERE id=$id");

print "$id was deleted";
print "<p><a href=/admin/qc/spam.php>continue</a></p>";

?>
	<script>
		function refresh() { document.location.href='/admin/qc/spam.php'; }
		setTimeout('refresh()',1000);
	</script>
<?

//-----------------------------------------
include('../admin_footer.inc');

