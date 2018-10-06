<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

//This file is used to reorder features in the DB. For now
//This does not do anything except make the admin page appear
//in a different order. In teh future if the fron tpage list of features is run from the
//DB this order number can be used for sorting those features
//(and the list on the nonadmin feature list page)

$display=false;
include('shared/global.cfg');
include('../admin_header.inc');
$page = new Page('category_reorder');
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
