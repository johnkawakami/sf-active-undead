<?php
//This page does bulk status changes

$display=true;
include('shared/global.cfg');
include("../admin_header.inc");
$page = new Page('article_bulk_status_change');
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
