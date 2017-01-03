<?php
// Reorders the features list

$display = false;
include('shared/global.cfg');
include('../admin_header.inc');
$page = new Page('feature_reorder');
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
