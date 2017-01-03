<?php
//This file is responsible for accepting a submission
//from the logon form and redirecting depending on success
//The includes for this file are not the standard includes for
//the admin pages since a user does not need to be logged on to run this code

$display = false;
include('shared/global.cfg');
$page = new Page('authenticate');
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
