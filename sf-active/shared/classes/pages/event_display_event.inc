<?php

// Class for the Calender Main Page...

class event_display_event extends Page {

    function event_display_event () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
		
	 	SF_CALENDAR_PATH."/common_include.inc";		
		
		// Translate all TPL_ in the Template
        $tr = new Translate();
        $tr->create_translate_table('event_display_event');		
		$this->translation($tr);		
		
		
			$event = new Event();
			$event = $event->find_by_id( $GLOBALS['caleventid'] );

			$this->tkeys['CAL_TITLE'] = htmlspecialchars ( $event->get_title() );	
			$this->tkeys['CAL_START_DATE'] = $event->get_start_month()."/".$event->get_start_day()."/".$event->get_start_year();

			$am = "AM";
			$start_time = $event->get_start_time();
			$hour = floor($start_time);

			if ($hour >= 12){
				$am = "PM";
			}

			if ($hour > 12){
				$hour = $hour-12;
			}

			if ($hour == 0){
				$hour = 12;
			}

			$minute = round(60 * ($start_time - floor($start_time) ) );
			if ($minute < 10){
				$minute = "0".$minute;
			}

			$this->tkeys['CAL_START_TIME'] =  $hour.":".$minute." ".$am;
			$this->tkeys['CAL_DURATION'] = $event->get_duration();
			$this->tkeys['CAL_LOCATION'] = htmlspecialchars ( $event->get_location_name() );


			// FIXME - better use the same way as in publish
			if ( strpos( $event->get_location_details(), "<" ) > 0)
			{
				$this->tkeys['CAL_LOCATION_DETAILS'] = $event->get_location_details();

			} else {

				$this->tkeys['CAL_LOCATION_DETAILS'] = nl2br( $event->get_location_details() );

			}

			$this->tkeys['CAL_TOPIC'] = htmlspecialchars( $event->get_event_topic_name() );
			$this->tkeys['CAL_TYPE'] = htmlspecialchars( $event->get_event_type_name() );
			$this->tkeys['CAL_CONTACT_NAME'] = htmlspecialchars( $event->get_contact_name() );
			$this->tkeys['CAL_CONTACT_EMAIL'] = htmlspecialchars( $event->get_contact_email() );
			$this->tkeys['CAL_CONTACT_PHONE'] = htmlspecialchars( $event->get_contact_phone() );

			// render the media. 
			if($event->get_file())
			{
			    // we borrow the article class
			    $art = new Article();
			    $art->article['created_month'] = $event->get_start_month();
			    $art->article['created_year'] = $event->get_start_year();
			    if($art->article['created_month'] < 10) $art->article['created_month'] = "0".$art->article['created_month'];
			    $art->article['mime_type'] = $event->get_mime_type();
			    $art->article['linked_file'] = $event->get_file();
			    $art->set_paths();
			    $art->set_mime_type();
			    $this->tkeys['cal_media'] = $art->article['media'] ;
			}else{
			    $this->tkeys['cal_media'] = '';
			}

/*			// FIXME - better use the same way as in publish
			if ( strstr($event->get_description(),"<") )
			{
				$this->tkeys['CAL_DESCRIPTION'] = $event->get_description();
			} else {
				$this->tkeys['CAL_DESCRIPTION'] = nl2br($event->get_description());
			}*/
			$cur_mime_type = $event->get_artmime();
			if($cur_mime_type == 'h')
			{
			    $this->tkeys['cal_description'] = $event->cleanup_html($event->get_description());
			}elseif($cur_mime_type == 't'){
			    $this->tkeys['cal_description'] = $event->cleanup_text($event->get_description());
			}else{
			    $this->tkeys['cal_description'] = $event->get_description();
			}
		
		
		return 1;		
	}
	
	function SetEventID ($eventid) {

		// Tell me a better way if you know one...
		$GLOBALS['caleventid'] = $eventid;	
	}

	function GetEventID () {
	
		return $GLOBALS['caleventid'];
	}

	function translation ($tr) {
		$this->tkeys['CAL_LANG_TITLE'] = $tr->trans('cal_title');
		$this->tkeys['CAL_LANG_START_DATE'] = $tr->trans('cal_start_date');
		$this->tkeys['CAL_LANG_START_TIME'] = $tr->trans('cal_start_time');
		$this->tkeys['CAL_LANG_DURATION'] = $tr->trans('cal_duration');
		$this->tkeys['CAL_LANG_HOURS'] = $tr->trans('cal_hours');
		$this->tkeys['CAL_LANG_LOCATION'] = $tr->trans('cal_location');		
		$this->tkeys['CAL_LANG_LOCATION_DETAILS'] = $tr->trans('cal_location_details');
		$this->tkeys['CAL_LANG_TOPIC'] = $tr->trans('cal_topic');
		$this->tkeys['CAL_LANG_TYPE'] = $tr->trans('cal_type');
		$this->tkeys['CAL_LANG_CONTACT_NAME'] = $tr->trans('cal_contact_name');
		$this->tkeys['CAL_LANG_CONTACT_PHONE'] = $tr->trans('cal_contact_phone');
		$this->tkeys['CAL_LANG_CONTACT_EMAIL'] = $tr->trans('cal_contact_email');
		$this->tkeys['CAL_LANG_DESCRIPTION'] = $tr->trans('cal_description');
	}
	
}
