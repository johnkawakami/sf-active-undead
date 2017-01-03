<?php

// Class for calendar_event_display_lookup

class calendar_event_display_lookup extends Page
{

    function calendar_event_display_lookup()
    {
        return 1;
    }

    function execute()
    {

        include_once(SF_SHARED_PATH . '/classes/calendar/event.inc');

        $event = new Event;
        $this->tkeys['local_results'] = '';
	$tr= new Translate;

        if (strlen($_POST['event_id']) > 0)
        {
            $event = $event->find_by_id($_POST['event_id']);
            if (is_object($event))
            {
                $return_html = $tr->trans('event_found');
                $return_html .= " " . $_POST['event_id'] . ":<BR>";
                $return_html .= $tr->trans('title') . $event->get_title() . "<BR>";
                $return_html .= $tr->trans('conf_no') . $event->get_confirmation_number();
                $this->tkeys['local_results'] = $return_html;
            }
        }

        return 1;
    }
}

?>