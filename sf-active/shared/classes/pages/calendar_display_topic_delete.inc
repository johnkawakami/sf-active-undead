<?php

// Class for calendar_display_topic_delete page

class calendar_display_topic_delete extends Page
{

    function calendar_display_topic_delete() {
        // Class constructor, does nothing
		$db = new DB ;
		if(isset($_GET['id']) > 0)
		{
			// update all events who use this event_topic we set the topic to 'other'.
			// if we don't do this, the events who use this event_topic can't be edited anymore.
			$result = $this->set_topics_to_other($_GET['id']);

			// delete the event_topic
			$sql = "delete from event_topic where event_topic_id = ".$_GET['id'];
			$db->execute_statement($sql);
			include_once(SF_CLASS_PATH.'/calendar/calendar.inc');
			$calendar_obj = new Calendar ;
			$calendar_obj->cache_event_topic_array();
							
		}
		header('Location: /admin/calendar/display_topic_list.php');
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

	function set_topics_to_other($id)
	{
		$db = new DB;
		$sql = "update event set event_topic_id = '0' where event_topic_id = '".$id."'";
		$result = $db->execute_statement($sql);
		return $result ;
	}

}

?>
