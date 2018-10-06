<?php
include 'shared/vendor/autoload.php';

include_once("shared/global.cfg");

$sftr = new Translate();
$page = new \SFACTIVE\Page('event_display_delete', array('noindex'=>true, 'canonical_url'=>'http://la.indymedia.org/calendar/event_display_delete.php'));

if ($page->get_error()) {
    echo "Fatal error: " . $page->get_error();
} else {
	$page->build_page();
	include(SF_INCLUDE_PATH."/content-header.inc");
	echo $page->get_html();
	include(SF_INCLUDE_PATH."/content-footer.inc"); 
}


