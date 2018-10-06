<?php

// Class for category_add page

class calendar_display_type_add extends Page
{

    function calendar_display_type_add()
    {
		$db = new DB;
		$next_id = $this->find_next_id() + 1 ;
		$sql = "insert into event_type values ('".$next_id."', '".$_POST['name']."')";
		$error_num = $db->execute_statement_return_autokey($sql);

        include_once(SF_CLASS_PATH.'/calendar/calendar.inc');
        $calendar_obj = new Calendar ;
        $calendar_obj->cache_event_type_array();
														

		$new_location = "display_type_edit.php?id=" . $next_id;

		header("Location: $new_location");
		exit;

    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

	function find_next_id()
	{
		$db = new DB ;
		$sql = "select max(event_type_id) as id from event_type" ;
		$result = $db->query($sql);
		while($row = array_pop($result))
		{
			$next_id = $row['id'];
		}
		return $next_id ;
	}

}

?>
