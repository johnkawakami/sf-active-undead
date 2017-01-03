<?php
//This file is used for wqriting out a cached file from teh stories for a feature in the DB

$display=false;
include('shared/global.cfg');
include('../admin_header.inc');
$page = new Page('category_pushtoproduction');
if ($page->get_error()) 
{
    echo "Fatal error: " . $page->get_error();
} else 
{
    $page->build_page();	
    echo $page->get_html();
}
include('../admin_footer.inc');

?>
