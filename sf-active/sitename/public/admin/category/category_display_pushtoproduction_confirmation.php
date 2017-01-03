<?php
//This page displays teh confirmation message that a page has been
//pushed live

$display=true;
include("shared/global.cfg");
include("../admin_header.inc");
$page = new Page('category_display_pushtoproduction_confirmation');
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
