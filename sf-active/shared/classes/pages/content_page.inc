<?php

// Class for content_page page
// This is a generic class to provide backwards-compatibility to any content page
// This should eventually be migrated to something which can be edited through the admin

class content_page extends Page {

    function admin_index() {
        // Class constructor, does nothing
        return 1;
    }

    function execute($content_page = '') {
        global $sftr;

        if (!is_object($sftr)) {
           $sftr = new Translate();
        }

         $langfile = SF_CACHE_PATH . "/pages/" . $content_page . "." . $GLOBALS['lang'] . ".inc";
         if (file_exists($langfile)) {
             
			 include($langfile);
			 
         } else {
             include(SF_CACHE_PATH . "/pages/" . $content_page . ".inc");
         }

        return 1;
    }

}

?>
