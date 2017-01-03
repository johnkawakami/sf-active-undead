<?php
//This file displays an edit page for a category
//this only includes things like the name and description and default
//values for new stories

$display=true;

include_once("shared/global.cfg");

$page = new Page('language_display_edit');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    include("../admin_header.inc");
	$page->build_page();	
    echo $page->get_html();
	include("../admin_footer.inc"); 
	
}

?>
