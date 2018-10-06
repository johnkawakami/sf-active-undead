<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

//This page is for looking up event confirmation numbers

$display=true;
include('shared/global.cfg');
include("../admin_header.inc");
$page = new Page('calendar_event_display_lookup');
if ($page->get_error()) 
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();
    echo $page->get_html();
}
include('../admin_footer.inc');

?>
