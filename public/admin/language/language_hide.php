<?php
//This file is used to add a feature to the DB (features are called Categories in the DB)
$display=false;
include_once('shared/global.cfg');

$page = new Page('language_hide');

if ($page->get_error()) 
{
    echo "Fatal error: " . $page->get_error();
} else 
{
	include('../admin_header.inc');
    $page->build_page();	
    echo $page->get_html();
	include('../admin_footer.inc');
}

?>
