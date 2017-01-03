<?php

// Class for feature_change_status page

class feature_change_status extends Page
{

    function feature_change_status()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');

        $feature_obj = new Feature;

	$feature_obj->updatestatus($_GET['feature_version_id'], $_GET['status']);

        header("Location: feature_display_list.php");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
