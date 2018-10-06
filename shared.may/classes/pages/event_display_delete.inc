<?php // vim:ai:et:sw=4:ts=4
// Class for the Calender Delte Event Page

include SF_CALENDAR_PATH."/common_include.inc";     

class event_display_delete extends Page {

    function event_display_delete ($settings=null) {
        $this->settings = $settings;
        $this->post = filter_input_array( INPUT_POST, [
            'event_id' => FILTER_SANITIZE_NUMBER_INT,
            'confirmation_number' => FILTER_SANITIZE_NUMBER_INT,
            'location_id' => FILTER_SANITIZE_NUMBER_INT
        ] );
        $this->post = coerce_array_values_to_int($this->post, [
            'event_id',
            'confirmation_number',
            'location_id'
        ] );
        return 1;
    }

    function execute () 
    {

        // Translate all TPL_ in the Template
        $tr = new Translate();
        $tr->create_translate_table('event_display_delete');        
        $this->translation($tr);

        $eid = $this->post['event_id'];

        if ($eid==null or $eid < 0) {
            http_response_code(301);
            header('Location: /calendar/');
            return;
        }

        $event= new \SFACTIVE\Calendar\Event();
        $event = $event->find_by_id($eid);      
        
        // Check Event
        if ( !$event == null )
        {
            $this->tkeys['CAL_EVENT_ID'] = $eid;
            
            // Check Conf Nr.
            $confnum = $this->post['confirmation_number'];
            if ( $confnum > 0 )
            {
                // Delete Event
                $error_num = $event->delete( $confnum );

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
                    $event->make_calendar_rdf($this->post['location_id']);
                    $event->make_calendar_minical();
                    $event->make_calendar_minical($this->post['location_id']);
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
        
