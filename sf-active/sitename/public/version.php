<?php
// This page dumps the current version number and then immediately quits

include_once("shared/global.cfg");

echo "sf-active version " . $GLOBALS['sfactive_version'];
exit;

?>
