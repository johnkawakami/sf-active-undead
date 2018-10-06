<?php
include 'shared/vendor/autoload.php';
include_once("shared/global.cfg");
include_once SF_CLASS_PATH."/firewall_class.inc";

// people are trying to hack this page - johnk
Firewall::protect();

$sftr = new Translate();

$event_id = filter_input( INPUT_GET, 'event_id', FILTER_SANITIZE_NUMBER_INT );
if (!$event_id) {
	http_response_code(400);
	$meta = array('noindex'=>true);
}

if ($event_id) {
	$meta =	array('canonical_url'=>'http://la.indymedia.org/calendar/event_display_detail.php?event_id='.$event_id);
} 


$page = new \SFACTIVE\Page('event_display_detail', $meta);

if ($page->get_error()) {
    echo "Fatal error: " . $page->get_error();
} else {
    $page->build_page();
	
	include(SF_INCLUDE_PATH."/content-header.inc");
    echo $page->get_html();
	include(SF_INCLUDE_PATH."/content-footer.inc"); 
}

