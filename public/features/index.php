<?php

//category_display_list.php displays a list of the features in the DB
//with drilldowns to lists of stories for each feature

include("shared/global.cfg");
include_once(SF_SHARED_PATH . '/classes/category_class.inc');
sf_include_file (SF_INCLUDE_PATH, 'content-header.inc');

$page = new Page('feature_list');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();
    echo $page->get_html();
}

sf_include_file (SF_INCLUDE_PATH, 'content-footer.inc');

