<?
// This file is used to regenerate archived features
$display=true;
include_once('shared/global.cfg');

$page = new Page('language_display_list');

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
