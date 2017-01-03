<?php

// Class for the Calender Detail Page...
class event_display_detail extends Page {

    function event_display_detail () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
	//This displays a cached file for an event		
		$get = filter_input_array(INPUT_GET, array(
			'day' => FILTER_VALIDATE_INT,
			'month' => FILTER_VALIDATE_INT,
			'year' => FILTER_VALIDATE_INT,
			'event_id' => FILTER_VALIDATE_INT,
		), TRUE);

		include SF_CALENDAR_PATH."/common_include.inc";		

		// Translate all TPL_ in the Template
		$tr = new Translate();
		$tr->create_translate_table('event_display_detail');		
		$this->translation($tr);

		$this->tkeys['CAL_DAY'] = $get['day'];
		$this->tkeys['CAL_MONTH'] = $get['month'];
		$this->tkeys['CAL_YEAR'] = $get['year'];
		$this->tkeys['CAL_EVENT_ID'] = $get['event_id'];
		
		if ( $get['event_id'] > 0 ) 
		{

			$filename = SF_CACHE_PATH."/calendar/event_cache/".$get['event_id'];
			if ( file_exists( $filename ) )
			{				
				$fcach=fopen($filename, "r");
				$fcachcontent = fread( $fcach, filesize($filename) );
				fclose($fcach);
				
				$this->tkeys['CAL_CACH_FILE'] = $fcachcontent;

			} else {
				
				// Fall back to DB
				$event_display = new EventDisplay();
				$this->tkeys['CAL_CACH_FILE'] = $event_display->renderEventHTML( $get['event_id'] );
				
			}			
		} else {
			$this->tkeys['CAL_CACH_FILE'] = "Error: no event id";
		}

	}

	function translation ($tr) {
		$this->tkeys['LANG_SITE_CRUMB'] = $GLOBALS['site_crumb'];

		$this->tkeys['CAL_LANG_CALENDAR'] = $tr->trans('cal_calendar');
		$this->tkeys['CAL_LANG_EVENT_DETAIL'] = $tr->trans('cal_event_detail');		
		$this->tkeys['CAL_LANG_RETURN_TO_CAL'] = $tr->trans('cal_return_to_cal');
		$this->tkeys['CAL_LANG_UPDATE'] = $tr->trans('cal_update');
		$this->tkeys['CAL_LANG_DELETE'] = $tr->trans('cal_delete');
	}
	
}

?>
