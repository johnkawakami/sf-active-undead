<?php

// Class for feature_delete page

class feature_delete extends Page
{

    function feature_delete()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

        //include(SF_SHARED_PATH . '/classes/feature_class.inc');

        global $db_obj;

        $query = "update feature set status='a' where feature_version_id=";
        $query = $query . $_GET['feature_version_id'];

        $db_obj->execute_statement($query);

        header("Location: feature_display_list.php");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>