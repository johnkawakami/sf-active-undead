<?php
//This page adds a user

$display=false;
include('shared/global.cfg');
include('../admin_header.inc');
include(SF_SHARED_PATH . '/classes/user_class.inc');
$page = new Page('user_add');
if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else {
    $page->build_page();	
    echo $page->get_html();
}
include('../admin_footer.inc');

?>
