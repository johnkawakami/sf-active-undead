<?php
// This is the page that handles content publishing from users
# session_start();
header("Content-type: text/html; charset=utf-8");

include_once("shared/global.cfg");

sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');

$page = new Page('publish');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();
    echo $page->get_html();
}

sf_include_file(SF_INCLUDE_PATH, 'content-footer.inc');

?>
