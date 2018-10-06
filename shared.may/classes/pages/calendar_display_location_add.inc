<?php

// Class for category_add page

class calendar_display_location_add extends Page
{

    function calendar_display_location_add()
    {
		$db = new DB;
		$next_id = $this->find_next_id() + 1 ;
		$sql = "insert into location values ('".$next_id."', '".$_POST['name']."')";
		$error_num = $db->execute_statement_return_autokey($sql);

		include(SF_SHARED_PATH."/classes/pages/syndication_index.inc");
        $syn = new Syndication_index ;
        $done = $syn->cache_syndication_list();

		$new_location = "display_location_edit.php?id=" . $next_id;

        include_once(SF_CLASS_PATH.'/calendar/calendar.inc');
        $calendar_obj = new Calendar ;
        $calendar_obj->cache_location_array();

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
		$sql = "select max(location_id) as id from location" ;
		$result = $db->query($sql);
		while($row = array_pop($result))
		{
			$next_id = $row['id'];
		}
		return $next_id ;
	}
}

?>
