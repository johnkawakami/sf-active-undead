<?php
//View and edit files in teh cache folder

$display=true;
include('shared/global.cfg');
include("../admin_header.inc");
$page = new Page('file_viewer');
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
