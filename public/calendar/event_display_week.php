<?php
include 'shared/vendor/autoload.php';
include_once("shared/global.cfg");
$left="noleft";
$sftr = new Translate();
$Title = $sftr->trans('calendar');

$page = new Page('event_display_week');

if ($page->get_error()) {
    echo "Fatal error: " . $page->get_error();
} else {
    $page->build_page();

	include(SF_INCLUDE_PATH."/content-header.inc");
    echo $page->get_html();
	include(SF_INCLUDE_PATH."/content-footer.inc"); 
}
