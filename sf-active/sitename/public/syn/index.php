<?php
//This page gives an overview of the different syndication files (per category). It is served from cache to avoid dbase overload.

$display=true;
include("shared/global.cfg");
sf_include_file (SF_INCLUDE_PATH, 'content-header.inc');

$page = new Page('syndication_index');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page('syndication_index');
    echo $page->get_html();
}

sf_include_file (SF_INCLUDE_PATH, 'content-footer.inc');

?>

