<?php
//This page displays the main list of calendar admin options

$display=false;
include("shared/global.cfg");
include("../admin_header.inc");
$page = new Page('calendar_display_type_add');
if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();	
    echo $page->get_html();
}
include("../admin_footer.inc"); 

?>
