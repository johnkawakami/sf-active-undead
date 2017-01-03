<?php
include("shared/global.cfg");

$page = new Page('xml');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();   
    echo $page->get_html();
}
  
?>
