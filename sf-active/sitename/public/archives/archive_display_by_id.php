<?php
//lists a page for each single story

include_once("shared/global.cfg");

$page = new Page('display_by_id');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();
	sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');
    echo $page->get_html();
	sf_include_file(SF_INCLUDE_PATH, 'content-footer.inc');
}

?>
