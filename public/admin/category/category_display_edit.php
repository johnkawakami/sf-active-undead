<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

//This file displays an edit page for a category
//this only includes things like the name and description and default
//values for new stories

$display=true;
include("shared/global.cfg");
include("../admin_header.inc");
$page = new Page('category_display_edit');
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
