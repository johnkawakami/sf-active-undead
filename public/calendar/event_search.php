<?php
include_once("shared/global.cfg");

if (!preg_match('/^\\d{0,2}$/', $_REQUEST['day'].$_REQUEST['limit'].$_REQUEST['month'].$_REQUEST['year'] )) { 
	header('HTTP/1.0 403 Forbidden');
	echo "Forbidden - invalid value for day parameter.";
	exit;
}

foreach(array('keyword','author','year','month','day','language_id','location','eventtype','eventtopic','sort','limit') as $k) {
	if (strlen($_REQUEST[$k]) > 20) {
		header('HTTP/1.0 403 Forbidden');
		echo "Forbidden - invalid parameter.";
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
