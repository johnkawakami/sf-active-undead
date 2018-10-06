<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

//This page displays the HTML for editing or adding a story to a feature.
//If the id is missing it is an "add" and if it is passed into this page
//it is an "update"

$display=true;
include_once('shared/global.cfg');
include("../admin_header.inc");
$page = new Page('feature_display_copy');
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
