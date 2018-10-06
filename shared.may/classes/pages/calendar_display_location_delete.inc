<?php

// Class for calendar_display_location_delete page

class calendar_display_location_delete extends Page
{

    function calendar_display_location_delete() {
        // Class constructor, does nothing
		$db = new DB ;
		if($_GET['id'] > 0)
		{
			// update all events who use this location. we set the location to 'other'.
			// if we don't do this, the events who use this location can't be edited anymore.
			$result = $this->set_locations_to_other($_GET['id']);

			//  deletes the location
			$sql = "delete from location where location_id = ".$_GET['id'];
			$db->execute_statement($sql);
		}
		include(SF_SHARED_PATH."/classes/pages/syndication_index.inc");
        $syn = new Syndication_index ;
        $done = $syn->cache_syndication_list();

       include_once(SF_CLASS_PATH.'/calendar/calendar.inc');
       $calendar_obj = new Calendar ;
       $calendar_obj->cache_location_array();
														
		
		header('Location: /admin/calendar/display_location_list.php');
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

	function set_locations_to_other($id)
	{
		$db = new DB;
		$sql = "update event set location_id = '0' where location_id = '".$id."'";
		$result = $db->execute_statement($sql);
		return $result ;
	}
}

?>
