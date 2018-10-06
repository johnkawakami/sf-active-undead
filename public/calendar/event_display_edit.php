<?php
include 'shared/vendor/autoload.php';
// Add and Update a Event

include_once("shared/global.cfg");

$sftr = new Translate();
$page = new Page('event_display_edit', array('noindex'=>true, 'canonical_url'=>'http://la.indymedia.org/calendar/event_display_edit.php'));

if ($page->get_error()) {
    echo "Fatal error: " . $page->get_error();
} else {
	$page->build_page();
	// Includes here so we can send a header from the class
	include(SF_INCLUDE_PATH."/content-header.inc");
	echo $page->get_html();
	include(SF_INCLUDE_PATH."/content-footer.inc"); 
}

