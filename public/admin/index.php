<?php
//This page displays the main list of admin options

$display=true;
include("shared/global.cfg");
include("admin_header.inc");
$page = new Page('category_display_list');
if ($page->get_error()) {
    echo "Fatal error: " . $page->get_error();
} else {
    $page->build_page();
    echo $page->get_html();
}
include("admin_footer.inc"); 

