<?php
// This page refreshes main/category newswires

$display = true;
include_once("shared/global.cfg");

ini_set('session.save_path', SF_SESSIONS_PATH);
session_start();
include(SF_WEB_PATH."/admin/admin_header.inc");
				
$page = new Page('refresh');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else {
    $page->build_page();
    echo $page->get_html();
}
include(SF_WEB_PATH."/admin/admin_footer.inc");
 
?>
