<?php

// Class for calendar_display_location_list.inc

class calendar_display_location_list extends Page {

    function calendar_display_location_list() {
        // Class constructor, does nothing
        return 1;
    }

    function execute() {
        // Execution method, does nothing
		$tr = new Translate();
        $db = new DB ;
		$sql = "select * from location order by location_id asc ";
		$location_list = $db->query($sql);
// added category id ot the listing showing off in the list -- blicero
        while ($nextlocation = array_pop($location_list))
        {
			$tblhtml .= "<tr><td><a href=\"display_location_edit.php?id=";
			$tblhtml .= $nextlocation['location_id'] . "\">";
			$tblhtml .= $nextlocation['location_id'] . "</a></td>";
			$tblhtml .= "<TD><A href=\"display_location_edit.php?id=";
			$tblhtml .= $nextlocation['location_id'] . "\">";
			$tblhtml .= $nextlocation['name'] . "</a>";
			$tblhtml .= "</td><td>";

			$tblhtml .= "<td>";
			if($nextlocation['location_id' ] > 0)
			{
				$tblhtml .= "<a href=\"display_location_delete.php?id=";
				$tblhtml .= $nextlocation['location_id'];
				$tblhtml .= "\">TPL_DELETE</a>";
			}
			$tblhtml .= "</td></tr>";
        }

        $this->tkeys['table_middle'] = $tblhtml;
        return 1;
    }

}

?>
