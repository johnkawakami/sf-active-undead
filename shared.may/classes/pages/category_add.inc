<?php

// Class for category_add page
include_once(SF_SHARED_PATH."/classes/category_class.inc");
class category_add extends Page
{

    function category_add()
    {
	if(strlen($_POST['shortname']) < 1)
	{
		header('Location: category_display_edit.php');
		exit;
	}
        // In this kind of script, we can do everything in the constructor to avoid the whole mess
        $category_obj = new \SFACTIVE\Category;
        $categoryfields =   array(
                                name =>                             $_POST['name'],
                                template_name =>                    $_POST['template_name'],
                                newswire =>                         $_POST['newswire'],
                                center =>                           $_POST['center'],
                                shortname =>                        $_POST['shortname'],
                                description =>                      $_POST['description'],
                                default_feature_template_name =>    $_POST['default_feature_template_name'],
                                order_num =>                        $_POST['order_num'],
// i added catclass where it exists -- blicero 20 may 2003
                                catclass =>                        $_POST['catclass']
                                );
				
        $error_num = $category_obj->add($categoryfields);
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
