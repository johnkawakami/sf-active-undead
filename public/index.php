<?php
//Index.php is the main page on the indymedia site
//It displays a cached center column and a cached summary list
//of the latest posts.

include("shared/global.cfg");

$sftr = new Translate();

$bottom = "cities";
$left   = SF_INCLUDE_PATH . "/left.inc";

sf_include_file (SF_INCLUDE_PATH, 'content-header.inc');
sf_include_file (SF_INCLUDE_PATH, 'index_center.inc');

echo $site_crumb; 
?> 

<a href="#top">(top)</a>

<?php

sf_include_file (SF_INCLUDE_PATH,  'center_right.inc'); 
sf_include_file(SF_INCLUDE_PATH, '/footer.inc');

