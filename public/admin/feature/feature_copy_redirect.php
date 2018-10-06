<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

// Copies a feature from one category to another

$display = false;
include('shared/global.cfg');
include('../admin_header.inc');
$page = new Page('feature_copy_redirect');
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
