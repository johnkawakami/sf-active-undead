<?php
// Class for the Calender Delte Event Page

class event_display_delete extends Page {

    function event_display_delete () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () 
	{
	
		include SF_CALENDAR_PATH."/common_include.inc";		

		// Translate all TPL_ in the Template
		$tr = new Translate();
		$tr->create_translate_table('event_display_delete');		
		$this->translation($tr);

		// Post / Get Mix...		
		if ($_GET['event_id'] > 0) 
		{
			$eid = $_GET['event_id'];

		} else if ($_POST['event_id'] > 0 ) {
		
			$eid = $_POST['event_id'];

		} else {

			$eid = "";		
		}

		$event= new Event();
		$event = $event->find_by_id($eid);		
		
		// Check Event
		if ( !$event == null )
		{
			$this->tkeys['CAL_EVENT_ID'] = $eid;
			
			// Check Conf Nr.
			if ( ( settype ( $_POST['confirmation_number'], "integer" ) )  &&
				 ( $_POST['confirmation_number'] > 0 )
			   )
			{
				
				// Delete Event
				$error_num = $event->delete( $_POST['confirmation_number'] );

				if ( $error_num < 0 )
				{
					// Error
					$this->tkeys['CAL_ERROR_MSG'] = $tr->trans('cal_error_event_delete');

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
					$event->make_calendar_rdf();
	                                $event->make_calendar_rdf($_POST['location_id']);
					$event->make_calendar_minical();
					$event->make_calendar_minical($_POST['location_id']);
					header("Location: event_display_week.php");
					exit;
				}
				
			} else {
			
				$this->tkeys['CAL_ERROR_MSG'] = "";
			}

		} else {
			
			$this->tkeys['CAL_ERROR_MSG'] = $tr->trans('cal_error_event_id');

		}
	}

	function translation ($tr) {
		$this->tkeys['CAL_LANG_ENTER_CONFIRM'] = $tr->trans('cal_enter_confirm');
		$this->tkeys['CAL_LANG_RETURN_TO_EVENT'] = $tr->trans('cal_return_to_event');		
		$this->tkeys['CAL_LANG_CONF_NO'] = $tr->trans('cal_conf_no');
		$this->tkeys['CAL_LANG_DELETE'] = $tr->trans('cal_delete');
	}
	
}
		
