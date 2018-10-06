<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

//This page displays an exmaple page of what the feature will look liek when pushed live
//this page also includes the link to push the page live

$display=true;
include("shared/global.cfg");
include("../admin_header.inc");
$page = new Page('category_display_preview');
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
