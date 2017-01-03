<?php
//This page refreshes the list of IMC cities.

$display=true;
include("shared/global.cfg");
include("../admin_header.inc");
$page = new Page('admin_cities');
if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
}
else
{
    $page->build_page();	
    echo $page->get_html();
}
include("../admin_footer.inc"); 

?>
