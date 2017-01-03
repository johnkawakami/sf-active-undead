<?php

include_once("shared/global.cfg");

$sftr = new Translate();

$page = new Page('event_display_ical');

if ($page->get_error()) {
    echo "Fatal error: " . $page->get_error();
} else {
    $page->build_page();
/*	
	include_once(SF_INCLUDE_PATH."/content-header.inc");
    echo $page->get_html();
	include_once(SF_INCLUDE_PATH."/content-footer.inc"); 
*/
}

?>
