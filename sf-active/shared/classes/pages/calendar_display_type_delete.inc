<?php

// Class for calendar_display_type_delete page

class calendar_display_type_delete extends Page
{

    function calendar_display_type_delete() {
        // Class constructor, does nothing
		$db = new DB ;
		if(isset($_GET['id']) > 0)
		{
			// update all events who use this event_type we set the event_type to 'other'.
			// if we don't do this, the events who use this event_type can't be edited anymore.
			$result = $this->set_types_to_other($_GET['id']);

			// delete the event_type
			$sql = "delete from event_type where event_type_id = ".$_GET['id'];
			$db->execute_statement($sql);

			include_once(SF_CLASS_PATH.'/calendar/calendar.inc');
            $calendar_obj = new Calendar ;
            $calendar_obj->cache_event_type_array();
							
		}
		header('Location: /admin/calendar/display_type_list.php');
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

	function set_types_to_other($id)
	{
		$db = new DB;
		$sql = "update event set event_type_id = '0' where event_type_id = '".$id."'";
		$result = $db->execute_statement($sql);
		return $result ;
	}

}

?>
