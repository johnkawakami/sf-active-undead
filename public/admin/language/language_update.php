<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

//the category update file is used for updating the meta information for a feature

$display=false;
include_once('shared/global.cfg');

$page = new Page('language_update');

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
