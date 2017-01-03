<?php
//This page is used for displaying the list of latest
//comments. New comments are added to the end of the
//file when a comment is added. No DB access occurs when
//latest comments are displayed

$display=true;
include("shared/global.cfg");
sf_include_file (SF_INCLUDE_PATH, 'content-header.inc');

$page = new Page('comment_latest');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page('comment_latest');
    echo $page->get_html();
}

sf_include_file (SF_INCLUDE_PATH, 'content-footer.inc');

?>
