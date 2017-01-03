<?php

// Class for calendar_display_topic_list.inc

class calendar_display_topic_list extends Page {

    function calendar_display_topic_list() {
        // Class constructor, does nothing
        return 1;
    }

    function execute() {
        // Execution method, does nothing
		$tr = new Translate();
        $db = new DB ;
		$sql = "select * from event_topic order by event_topic_id asc ";
		$event_topic_list = $db->query($sql);
// added category id ot the listing showing off in the list -- blicero
        while ($nextevent_topic = array_pop($event_topic_list))
        {
			$tblhtml .= "<tr><td><a href=\"display_topic_edit.php?id=";
			$tblhtml .= $nextevent_topic['event_topic_id'] . "\">";
			$tblhtml .= $nextevent_topic['event_topic_id'] . "</a></td>";
			$tblhtml .= "<TD><A href=\"display_topic_edit.php?id=";
			$tblhtml .= $nextevent_topic['event_topic_id'] . "\">";
			$tblhtml .= $nextevent_topic['name'] . "</a>";
			$tblhtml .= "</td><td>";

			$tblhtml .= "<td>";
			if($nextevent_topic['event_topic_id'] > 0)
			{
				$tblhtml .= "<a href=\"display_topic_delete.php?id=";
				$tblhtml .= $nextevent_topic['event_topic_id'];
				$tblhtml .= "\">TPL_DELETE</a>";
			}
			$tblhtml .= "</td></tr>";
        }

        $this->tkeys['table_middle'] = $tblhtml;
        return 1;
    }

}

?>
