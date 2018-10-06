<?php

// Class for category_update page
include_once(SF_SHARED_PATH."/classes/category_class.inc");
class category_update extends Page
{

    function category_update()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

        $category_obj=new \SFACTIVE\Category;

        $categoryfields = array(
                                'category_id' => $_POST["category_id"],
                                'name' => $_POST["name"],
                                'template_name' => $_POST["template_name"],
                                'default_feature_template_name' => $_POST["default_feature_template_name"],
                                'order_num' => $_POST["order_num"],
                                'shortname' => $_POST["shortname"],
                                'summarylength' => $_POST["summarylength"],
                                'parentid' => $_POST["parentid"],
                                'newswire' => $_POST["newswire"],
                                'center' => $_POST["center"],
                                'description' => $_POST["description"],
// i added catclass to the array where is needed
                                'catclass' => $_POST["catclass"]
                                );

        $error_num = $category_obj->update($categoryfields);

//add by bart -- 21 may 2003 
        include(SF_SHARED_PATH."/classes/pages/syndication_index.inc");
        $syn = new Syndication_index ;
        $done = $syn->cache_syndication_list();

        $new_location = "category_display_edit.php?category_id=" . $_POST["category_id"];

        header("Location: $new_location");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
