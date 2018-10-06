<?php // vim:et:ai:ts=4:sw=4

// Class for calendar_display_type_list.inc

class calendar_display_type_list extends Page {

    function calendar_display_type_list() {
        // Class constructor, does nothing
        return 1;
    }

    function execute() {
        // Execution method, does nothing
		$tr = new Translate();
        $db = new SFACTIVE\DB ;
		$sql = "select * from event_type order by event_type_id asc ";
		$event_type_list = $db->query($sql);
// added category id ot the listing showing off in the list -- blicero
        while ($nextevent_type = array_pop($event_type_list))
        {
			$tblhtml .= "<tr><td><a href=\"display_type_edit.php?id=";
			$tblhtml .= $nextevent_type['event_type_id'] . "\">";
			$tblhtml .= $nextevent_type['event_type_id'] . "</a></td>";
			$tblhtml .= "<TD><A href=\"display_type_edit.php?id=";
			$tblhtml .= $nextevent_type['event_type_id'] . "\">";
			$tblhtml .= $nextevent_type['name'] . "</a>";
			$tblhtml .= "</td><td>";

			$tblhtml .= "<td>";
			if($nextevent_type['event_type_id'] > 0)
			{
				$tblhtml .= "<a href=\"display_type_delete.php?id=";
				$tblhtml .= $nextevent_type['event_type_id'];
				$tblhtml .= "\">TPL_DELETE</a>";
			}
			$tblhtml .= "</td></tr>";
        }

        $this->tkeys['table_middle'] = $tblhtml;
        return 1;
    }

}

?>
