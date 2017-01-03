<?php
// This page shows a list of calendars for the archives

include_once("shared/global.cfg");
$display=false;
$page = new Page('dossier_feature_add');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();
	
	sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');
    echo $page->get_html();
	sf_include_file(SF_INCLUDE_PATH, 'content-footer.inc');
}

?>
