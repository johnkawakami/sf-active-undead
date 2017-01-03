<?php
include("shared/global.cfg");
$_GET[hidden]="yes";
$_GET[display]="f";

sf_include_file (SF_INCLUDE_PATH, 'content-header.inc');

$page = new Page('hidden');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();   
    echo $page->get_html();
}

sf_include_file (SF_INCLUDE_PATH, 'content-footer.inc');
  
?>
