<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

//This page displays the main list of admin options

$display=true;
include("shared/global.cfg");
include("../admin_header.inc");
$page = new \SFACTIVE\Page('upload_display_add');
if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else {
    $page->build_page();	
    echo $page->get_html();
}
include("../admin_footer.inc"); 

?>
