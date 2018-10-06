<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

//category_display_list.php displays a list of the features in the DB
//with drilldowns to lists of stories for each feature

$display=true;
include("shared/global.cfg");
include("../admin_header.inc");
$page = new Page('category_display_list');

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
