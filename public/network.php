<?php
//This page displays the list of imc sites

include_once("shared/global.cfg");

sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');

$page = new Page('network');

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
