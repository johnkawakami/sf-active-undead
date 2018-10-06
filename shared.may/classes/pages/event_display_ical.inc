<?php
/**
 *  displays an ical file of an event.
 *  @package	unused
 */
class event_display_ical extends Page
{

    /**
     *	Class constructor, does nothing
     */
    function event_display_ical()
    {
        return 1;
    }

    /**
     *	displays an ical file of an event
     */
    function execute()
    {

	if ($_GET['event_id'] > 0) {
		header("Content-type: text/calendar");
		header('Content-Disposition: attachment; filename=event_' .  $_GET['event_id']. '.ics');
		
		$filename = SF_CACHE_PATH . '/calendar/event_cache/event_' . $_GET['event_id'] . '.ics';

		if (file_exists($filename)) {
			print (file_get_contents($filename)); 
		} else { // fallback on db
			include_once(SF_SHARED_PATH . '/classes/calendar/ical.inc');
			include_once(SF_SHARED_PATH . '/classes/calendar/event.inc');
			$EventObj = new Event();
			$event = $EventObj->find_by_id($_GET['event_id']);
			$ical = new iCal(Array(0=>$event), $GLOBALS['site_name']);
			print ($ical->display_vcal());
		}
			
	}

        return 1;
    }

} // end class
?>
