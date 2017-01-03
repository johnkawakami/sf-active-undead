<?php
// displays a list of features for a given category

$display=true;
include('shared/global.cfg');
include("../admin_header.inc");
$page = new Page('feature_display_list');
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
