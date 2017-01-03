<?php

// Class for category_update page
class calendar_display_location_update extends Page
{

    function calendar_display_location_update()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

        $db = new DB;
		$sql = "update location set name = '".$_POST['name']."' where location_id = ".$_POST['id'];
        $error_num = $db->execute_statement($sql);

		include(SF_SHARED_PATH."/classes/pages/syndication_index.inc");
        $syn = new Syndication_index ;
        $done = $syn->cache_syndication_list();

		include_once(SF_CLASS_PATH.'/calendar/calendar.inc');
		$calendar_obj = new Calendar ;
		$calendar_obj->cache_location_array();

        $new_location = "display_location_edit.php?id=" . $_POST["id"];

        header("Location: $new_location");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
