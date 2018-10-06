<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

// This reorders the languages in the table language.
// The select_language.inc file is ordered on the ordernumbers.

$display=false;
include_once('shared/global.cfg');

$page = new Page('language_reorder');

if ($page->get_error()) 
{
    echo "Fatal error: " . $page->get_error();
} else 
{
    include('../admin_header.inc');
	$page->build_page();	
    echo $page->get_html();
	include('../admin_footer.inc');
}

?>
