<?php
//this page is the template for all category index.php
//It displays a cached center column and a cached summary list
//of the latest posts.

include("shared/global.cfg");

$sftr = new Translate();

$GLOBALS['page_title']='TPL_TITLE';
$bottom = "cities";
$left   = SF_INCLUDE_PATH . "/left.inc";
$feature_name="TPL_FEATURE_NAME";
$GLOBALS["category_id"]="TPL_CATID";
$category_id="TPL_CATID";

sf_include_file (SF_INCLUDE_PATH, 'content-header.inc');
include(SF_INCLUDE_PATH. '/index_center.inc');

echo $site_crumb; 
?> 

<a href="#top">(top)</a>

<?php

include(SF_INCLUDE_PATH.'/center_right.inc'); 
include(SF_INCLUDE_PATH.'/footer.inc');

?>
