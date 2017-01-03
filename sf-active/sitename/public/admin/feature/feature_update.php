<?php
//This file is used for updating a story in the DB. Since all versions of a story are
//preserved this is actually and insert plus an update of teh status of the old version
//to be noncurrent

$display = false;
include('shared/global.cfg');
include('../admin_header.inc');
$page = new Page('feature_update');
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
