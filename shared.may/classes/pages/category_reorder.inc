<?php

// Class for category_reorder page
include_once(SF_SHARED_PATH."/classes/category_class.inc");
class category_reorder extends Page
{

    function category_reorder()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

        $category_obj = new \SFACTIVE\Category;
        $error_num = $category_obj->reorder($_GET);

//add by bart -- 21 may 2003 
        include(SF_SHARED_PATH."/classes/pages/syndication_index.inc");
        $syn = new Syndication_index ;
        $done = $syn->cache_syndication_list();
	
	header("Location: category_display_list.php");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
