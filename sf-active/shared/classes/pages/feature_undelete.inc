<?php

// Class for feature_reorder page

class feature_reorder extends Page
{

    function feature_reorder()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

        include(SF_SHARED_PATH . '/classes/feature_class.inc');

        $feature_obj = new Feature;
        $error_num = $feature_obj->reorder($_GET);

        header("Location: feature_display_list.php");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>