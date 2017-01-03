<?php

// Class for feature_copy_redirect page

class feature_copy_redirect extends Page
{

    function feature_copy_redirect()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');
        include_once(SF_SHARED_PATH . '/classes/category_class.inc');

        $category_id = $_POST['new_category_id'];
        $feature_version_id = "";

        $category_obj = new Category();
        $category_fields = $category_obj->get_category_fields($_POST['category_id']);

        $category_name = $category_fields['name'];
        $summary = str_replace("\\\"", "\"", $summary);

        header("Location: feature_display_edit.php");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>