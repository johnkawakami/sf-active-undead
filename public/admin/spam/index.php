<?php
// Spam page

$display=true;
include("shared/global.cfg");
include("../admin_header.inc");
$page = new Page('spam');
if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();
    echo $page->get_html();
    echo "<li><a href=/admin/log/file_viewer.php?filename=spam_strings.txt>SPAM STRINGS</a>";
}
include("../admin_footer.inc"); 

