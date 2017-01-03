<?php

class calendar_delete extends Page
{

	function calendar_delete()
	{
		return 1;
	}

	function execute()
	{
                include SF_CALENDAR_PATH."/common_include.inc";
                // Post / Get Mix...
                if ($_GET['event_id'] > 0)
                {
                        $eid = $_GET['event_id'];
                }else{
		    $new_location = SF_ADMIN_URL.'/calendar';
		    header("Location: $new_location");
		    exit;
		} 

                $event= new Event();
                $event = $event->find_by_id($eid);

                // Check Event
                if ( !$event == null )
                {
		    $this->tkeys['CAL_EVENT_ID'] = $eid;

		    //  get delete info
		    $event->set_this_delete_info($eid);

                    // Delete Event
                    $error_num = $event->delete($event->delete_info["confirmation_number"]);
                    if ( $error_num < 0 )
                    {
                        // Error
			$new_location = SF_ADMIN_URL.'/calendar';
			header("Location: $new_location");
			exit;
                    } else {
                        // Ok Finish Event Delete
                        // Update the Week Cache
                        $event->createWeekCache ( $event->get_start_date_object() );

                        // Delete the Cache File of the Event.
                        // event id should be ok because we are here and $event is <> null...
                        $filename = SF_CACHE_PATH."/calendar/event_cache/".$eid;
                        if ( file_exists( $filename ) )
                        {
                            unlink ($filename);
                        }
                            $event = new Event();
			    $event->make_calendar_minical();
			    $event->make_calendar_minical($event->delete_info["location"]);
                            $event->make_calendar_rdf();
                            $event->make_calendar_rdf($event->delete_info["location"]);

			    $new_location = SF_ADMIN_URL.'/calendar';
                            header("Location: $new_location");
                            exit;
                    }

                }else{
		    $new_location = SF_ADMIN_URL.'/calendar';
                    header("Location: $new_location");
                    exit;
		}
 	}
}
?>
