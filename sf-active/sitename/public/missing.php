<?php
//missing.php is the page that is displayed when a page can not be found
//Generally, it is safe to remove this file ... it is mostly used for sf-imc
//You can use it also, but how to do it is undocumented - but it is better for
//search engine placement, you can bet us on that one.
//(Apache must be configured for this to work)

$request = $_SERVER['REQUEST_URI'];
include('shared/global.cfg');
include(SF_INCLUDE_PATH . '/missing.inc');
?>
