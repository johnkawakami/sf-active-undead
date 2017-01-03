<?php
// Paging newswire & search
include_once("shared/global.cfg");
$page = new Page('gallery');
$display = false; 

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
}
else
{
    $page->build_page();
    echo $page->get_html();
}
?>
