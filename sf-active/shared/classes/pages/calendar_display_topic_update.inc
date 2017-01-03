<?php

// Class for category_update page
class calendar_display_topic_update extends Page
{

    function calendar_display_topic_update()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

        $db = new DB;
		$sql = "update event_topic set name = '".$_POST['name']."' where event_topic_id = ".$_POST['id'];
        $error_num = $db->execute_statement($sql);

		include_once(SF_CLASS_PATH.'/calendar/calendar.inc');
		$calendar_obj = new Calendar ;
		$calendar_obj->cache_event_topic_array();
						

        $new_event_topic = "display_topic_edit.php?id=" . $_POST["id"];

        header("Location: $new_event_topic");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
