<?php
//This file is used to add a feature to the DB (features are called Categories in the DB)
$display=false;
include('shared/global.cfg');
include('../admin_header.inc');
$page = new Page('category_add');
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
