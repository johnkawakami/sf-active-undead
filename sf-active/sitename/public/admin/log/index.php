<?php
// displays a list of form to edit or add a template, include or css

$display=true;
include("shared/global.cfg");
include("../admin_header.inc");
$page = new Page('logedit_index');
if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();	
    echo $page->get_html('logedit_index');
}
include("../admin_footer.inc");

?>
