<?php

// Class for calendar_event_refresh_all

class calendar_event_refresh_all extends Page
{

    function calendar_event_refresh_all()
    {
        return 1;
    }

    function execute()
    {
        $tr = new Translate();
        $tr->create_translate_table(calendar);
        $this->tkeys['location_menu'] = $tr->trans(location_menu);
        $this->tkeys['type_menu'] = $tr->trans(type_menu);
        $this->tkeys['topic_menu'] = $tr->trans(topic_menu);
        $this->tkeys['regen_event'] = $tr->trans(regen_event);
        $this->tkeys['lookup_conf'] = $tr->trans(lookup_conf);
	$tr = new Translate();
	$tr->create_translate_table(event_refresh_all);
	$this->tkeys[admin_index]=$tr->trans(admin_index);
	$this->tkeys[admin_calendar]=$tr->trans(admin_calendar);
		
        include_once('../../calendar/common_include.inc');
	
        $db_obj = new DB();
        $event = new Event;
        $all_events = $event->find_all();
        $lastweek = "";

        if (sizeof($all_events) > 0)
        {
            while ($next_event = array_pop($all_events))
           {
		$diocane=$next_event->createEventCache ( $next_event->get_id() );	
                if ($lastweek != $next_event->get_weekname())
                {
			$this->tkeys[event_refresh].=$tr->trans('refresh_event').$next_event->get_weekname()."<br>";
			$next_event->createWeekCache( $next_event->get_start_date_object() );
                }

                $lastweek = $next_event->get_weekname();
            }
	$this->tkeys[event_up]=$tr->trans(event_up);
        }

        return 1;
    }
}

?>
