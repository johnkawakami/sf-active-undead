<?php
// This page creates the archived cache files for feature archives

include_once("shared/global.cfg");

//sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');

$page = new Page('archive_generate_weeks');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();
    //echo $page->get_html();
}

//sf_include_file(SF_INCLUDE_PATH, 'content-footer.inc');

?>
