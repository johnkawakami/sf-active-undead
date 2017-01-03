<?php
//This file is used for displaying, adding and confirming
//the adding of commnets to posts (and other comments)
# session_start();
header("Cache-control: max-age=3600");
header("Pragma: cache");
header("Content-type: text/html; charset=utf-8");
include_once("shared/global.cfg");

if (!is_numeric($_GET['top_id'])) {
	echo(urldecode($_SERVER['QUERY_STRING']));
	exit;
}

sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');

$page = new Page('comment');

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
