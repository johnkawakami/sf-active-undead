<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

//This page displays the history of a given story
//(that is all versions of it stored in the DB)

$display=true;
include('shared/global.cfg');
include("../admin_header.inc");
$page = new Page('feature_display_history');
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
