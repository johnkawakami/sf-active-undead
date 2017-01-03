<?php
// This is the login page
$display=true;
$display_logon=true;
include("shared/global.cfg");

// code to logout.  i'm putting it here for now.
ini_set('session.save_path', SF_SESSIONS_PATH);
session_start();
session_destroy();

$page = new Page('authenticate_display_logon');

if ($page->get_error()) 
{
    echo "Fatal error: " . $page->get_error();
} else 
{
	sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');
    $page->build_page();	
    echo $page->get_html();
	sf_include_file(SF_INCLUDE_PATH, 'content-footer.inc');
}

?>
