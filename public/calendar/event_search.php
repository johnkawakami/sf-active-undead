<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';
include_once("shared/global.cfg");
include_once(SF_CLASS_PATH.'/firewall_class.inc');

Firewall::protect();

if (!preg_match('/^\\d{0,10}$/', $_REQUEST['day'].$_REQUEST['limit'].$_REQUEST['month'].$_REQUEST['year'] )) { 
	header('HTTP/1.0 403 Forbidden');
	echo "Forbidden - invalid value for day parameter.";
	Firewall::add($_SERVER['REMOTE_ADDR']);
	exit;
}

foreach(array('keyword','author','year','month','day','language_id','location','eventtype','eventtopic','sort','limit') as $k) {
	if (strlen($_REQUEST[$k]) > 20) {
		header('HTTP/1.0 403 Forbidden');
		echo "Forbidden - invalid parameter.";
		Firewall::add($_SERVER['REMOTE_ADDR']);
		exit;
	}
}

$sftr = new Translate();
$Title = $sftr->trans('calendar');

$page = new Page('event_search');

if ($page->get_error()) {
    echo "Fatal error: " . $page->get_error();
} else {
    $page->build_page();

	include (SF_INCLUDE_PATH."/content-header.inc");
	include (SF_INCLUDE_PATH."/content-crumblink.inc"); 
	echo $page->get_html();
	include (SF_INCLUDE_PATH."/content-crumblink.inc"); 
	include (SF_INCLUDE_PATH."/content-footer.inc"); 
	
}
?>
