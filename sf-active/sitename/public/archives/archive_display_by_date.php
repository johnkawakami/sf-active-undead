<?php
//This file displays a reverse chronological paged view of the archives stories for a feature
//display_by_date_db is included for the DB based portion of this page

include_once("shared/global.cfg");

$page = new Page('archive_display_by_date');

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
