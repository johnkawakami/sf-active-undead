<?php
//This page shows the mailing list page for sending
//out email to the indymedia weely email list

include_once("shared/global.cfg");

sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');

$page = new Page('mailinglist');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else {
    $page->build_page();
    echo $page->get_html();
}

sf_include_file(SF_INCLUDE_PATH, 'content-footer.inc');

?>
