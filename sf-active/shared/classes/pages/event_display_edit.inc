<?php

// Class for the Calender Add and Update Form

class event_display_edit extends Page {

    function event_display_edit () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
	
		include SF_CALENDAR_PATH."/common_include.inc";
		
		// Translate all TPL_ in the Template
        	$tr = new Translate();
        	$tr->create_translate_table('event_display_edit');		
		$tr->create_translate_table('publish');
		$this->translation($tr);

		$this->types_array = array(
			    " " => " ",
                            "application/pdf" =>               "application/pdf",
                            "application/smil" =>              "application/smil",
                            "application/x-shockwave-flash" => "application/x-shockwave-flash",
                            "audio/mpeg" =>                    "audio/mpeg",
                            "audio/x-mpegurl" => "audio/x-mpegurl",
                            "audio/x-ms-wma" => "audio/x-ms-wma",
                            "audio/x-ogg" => "audio/x-ogg",
                            "audio/x-pn-realaudio" =>          "audio/x-pn-realaudio",
                            "audio/x-pn-realaudio-meta" => "audio/x-pn-realaudio-meta",
                            "audio/x-scpls" => "audio/x-scpls",
                            "audio/x-wav" => "audio/x-wav",
                            "image/gif" =>                     "image/gif",
                            "image/jpeg" =>                    "image/jpeg",
                            "image/png" =>                     "image/png",
                            "text/html" =>                     "text/html",
                            "text/plain" =>                    "text/plain",
                            "video/mpeg" =>                    "video/mpeg",
                            "video/quicktime" =>               "video/quicktime",
                            "video/x-ms-wmv" => "video/x-ms-wmv",
                            "video/x-msvideo" => "video/x-msvideo",
                            "video/x-pn-realvideo" =>          "video/x-pn-realvideo",
                            "video/x-pn-realvideo-meta" => "video/x-pn-realvideo-meta"
		    );

		// Edit Event...
		if ( $_POST['caction']  == "" && strlen($_GET['event_id']) > 0 )
		{
			$this->GetEventDatafromDB($tr);
		}
		

		if ( $_POST['caction']  == "update" && strlen($_POST['event_id']) > 0 )
		{
			$this->UpdateEventtoDB($tr);
			//echo "Update";
		}

		return 1;		
	}
		
	
	function GetEventDatafromDB ($tr) 
	{
	
	
		// Init		
		
		$event = new Event();
		
		$location = new Location();
		$event_topic = new EventTopic();
		$event_type = new EventType();
		$event_renderer = new EventRenderer();
		$date_renderer = new DateRenderer();
                include(SF_CACHE_PATH.'/location_options.inc');
                include(SF_CACHE_PATH.'/event_type_options.inc');
                include(SF_CACHE_PATH.'/event_topic_options.inc');

		$s = array('<', '>', '"');
		$r = array('&lt;', '&gt;', '&quot;');

		$event = $event->find_by_id($_GET['event_id']);
		$this->tkeys['CAL_PTITLE'] = str_replace($s, $r, $event->get_title());
		$this->tkeys['CAL_EVENT_ID'] = $_GET['event_id'];
		$this->tkeys['CAL_ACTION_TITLE'] = $tr->trans('cal_update');
		$this->tkeys['CAL_FORM_NAME'] = "event_update";
		$this->tkeys['CAL_FORM_FILE'] = "event_display_edit.php";
		$this->tkeys['CAL_CACTION'] = "update";

		// Date
		$start_day = $event->get_start_day();
		$start_month = $event->get_start_month();
		$start_year = $event->get_start_year();
		$start_hour = floor($event->get_start_time());
		$am = 0;
		$start_minute = 60*($event->get_start_time()-$start_hour);

		// 12 Hour Stuff
		if ($start_hour >= 12)
		{
			$am="12";
		}

		if ($start_hour > 12 )
		{
			$start_hour=$start_hour-12;
		}

		if ($start_hour==0)
		{
			$start_hour=12;
		}

		// Start Drop Downs...
		$this->tkeys['CAL_DD_START_MONTH'] = $event_renderer->create_dropdown($date_renderer->get_months(),$start_month);			
		$this->tkeys['CAL_DD_START_DAY'] = $event_renderer->create_dropdown($date_renderer->get_days_of_month(),$start_day);
		$this->tkeys['CAL_DD_START_YEAR'] = $event_renderer->create_dropdown($date_renderer->get_years(),$start_year);
		$this->tkeys['CAL_DD_START_HOUR'] = $event_renderer->create_dropdown($date_renderer->get_hours(),$start_hour);
		$this->tkeys['CAL_DD_START_MINUTE'] = $event_renderer->create_dropdown($date_renderer->get_minutes(),$start_minute);
		$this->tkeys['CAL_DD_AMPM'] = $event_renderer->create_dropdown($date_renderer->get_ampm(), $am );
		
		// Get Data and set templates
		$this->tkeys['CAL_PDURATION'] = $event->get_duration();

		// Location
		$this->tkeys['CAL_DD_LOCATION_ID'] = $this->make_select_form('location_id', $location_options, $event->get_location_id() );
		$this->tkeys['CAL_LOCATION_OTHER'] = str_replace($s, $r, $event->get_location_name());
		$this->tkeys['CAL_LOCATION_DETAILS'] = str_replace($s, $r, $event->get_location_details());

		// Event Type
		$this->tkeys['CAL_DD_EVENT_TYPE_ID'] = $this->make_select_form('event_type_id', $event_type_options, $event->get_event_type_id() );
		$this->tkeys['CAL_EVENT_TYPE_OTHER'] = str_replace($s, $r, $event->get_event_type_name());

		// Event Topic
		$this->tkeys['CAL_DD_EVENT_TOPIC_ID'] = $this->make_select_form('event_topic_id', $event_topic_options, $event->get_event_topic_id() );
		$this->tkeys['CAL_EVENT_TOPIC_OTHER'] = str_replace($s, $r, $event->get_event_topic_name());

		// Contact
		$this->tkeys['CAL_CONTACT_NAME'] = str_replace($s, $r, $event->get_contact_name());
		$this->tkeys['CAL_CONTACT_PHONE'] = str_replace($s, $r, $event->get_contact_phone());
		$this->tkeys['CAL_CONTACT_EMAIL'] = str_replace($s, $r, $event->get_contact_email());

		$this->tkeys['CAL_DISCRIPTION'] = str_replace($s, $r, $event->get_description());

		$this->tkeys['CAL_ERROR_MSG'] = "";
                $this->tkeys['upload_max_filesize'] = ini_get('upload_max_filesize');
                $this->tkeys['post_max_size'] = ini_get('post_max_size');
                $this->tkeys['max_execution_time'] = ini_get('max_execution_time')/60;
		$this->tkeys['cal_linked_file'] = $event->get_file();
		$this->tkeys['cal_mime_type'] = $this->make_select_form('mime_type', $this->types_array, $event->get_mime_type());
		include_once(SF_CACHE_PATH.'/language_options.inc');
		$language_options[''] = ' '.$tr->trans('select_a_language');
		asort($language_options);
		$this->tkeys['local_language'] = $this->make_select_form('language_id', $language_options, $event->get_language_id());
                $text_options = array(
                    "t" => "text/plain",
                    "h" => "text/html"
                );
                $this->tkeys['select_artmime'] = $this->make_select_form('artmime', $text_options, $event->get_artmime());
	}


	function GetEventDatafromPOST ($tr)
	{
	// Get the Post Data Back to the Form if there is a Error	

		// We need that to create the drop downs
		$event = new Event();
                include(SF_CACHE_PATH.'/location_options.inc');
                include(SF_CACHE_PATH.'/event_type_options.inc');
                include(SF_CACHE_PATH.'/event_topic_options.inc');

		$location = new Location();
		$event_topic = new EventTopic();
		$event_type = new EventType();
		$event_renderer = new EventRenderer();
		$date_renderer = new DateRenderer();

	
		// Set Form Action Stuff
		if ($_POST['caction'] == "update") 
		{
			$this->tkeys['CAL_ACTION_TITLE'] = $tr->trans ('cal_update');
			$this->tkeys['CAL_FORM_NAME'] = "event_update";
			$this->tkeys['CAL_FORM_FILE'] = "event_display_edit.php";
			$this->tkeys['CAL_CACTION'] = $_POST['caction'];

		} else if ($_POST['caction'] == "addnew") {

			$this->tkeys['CAL_ACTION_TITLE'] = $tr->trans ('cal_add');
			$this->tkeys['CAL_FORM_NAME'] = "event_add";
			$this->tkeys['CAL_FORM_FILE'] = "event_display_edit.php";
			$this->tkeys['CAL_CACTION'] = $_POST['caction'];
		
		}

		$this->tkeys['CAL_PTITLE'] = $_POST['title'];
		$this->tkeys['CAL_PDURATION'] = $_POST['duration'];

		$this->tkeys['CAL_DD_LOCATION_ID'] = $this->make_select_form('location_id', $location_options,  $_POST['location_id'] );
		$this->tkeys['CAL_LOCATION_OTHER'] = $_POST['location_other'];
		$this->tkeys['CAL_LOCATION_DETAILS'] = $_POST['location_details'];

		// Event Type
		$this->tkeys['CAL_DD_EVENT_TYPE_ID'] = $this->make_select_form('event_type_id', $event_type_options, $_POST['event_type_id'] );
		$this->tkeys['CAL_EVENT_TYPE_OTHER'] = $_POST['event_type_other'];

		// Event Topic
		$this->tkeys['CAL_DD_EVENT_TOPIC_ID'] = $this->make_select_form('event_topic_id', $event_topic_options, $_POST['event_topic_id'] );
		$this->tkeys['CAL_EVENT_TOPIC_OTHER'] = $_POST['event_topic_other'];

		// Contact
		$this->tkeys['CAL_CONTACT_NAME'] = $_POST['contact_name'];
		$this->tkeys['CAL_CONTACT_PHONE'] = $_POST['contact_phone'];
		$this->tkeys['CAL_CONTACT_EMAIL'] = $_POST['contact_email'];

		$this->tkeys['CAL_DISCRIPTION'] = $_POST['description'];


		$this->tkeys['CAL_DD_START_MONTH'] = $event_renderer->create_dropdown($date_renderer->get_months(), $_POST['start_month']);
		$this->tkeys['CAL_DD_START_DAY'] = $event_renderer->create_dropdown($date_renderer->get_days_of_month(), $_POST['start_day']);
		$this->tkeys['CAL_DD_START_YEAR'] = $event_renderer->create_dropdown($date_renderer->get_years(), $_POST['start_year']);
		
		$this->tkeys['CAL_DD_START_HOUR'] = $event_renderer->create_dropdown($date_renderer->get_hours(), $_POST['start_hour']);
		$this->tkeys['CAL_DD_START_MINUTE'] = $event_renderer->create_dropdown($date_renderer->get_minutes(), $_POST['start_minute']);

		$this->tkeys['CAL_DD_AMPM'] = $event_renderer->create_dropdown($date_renderer->get_ampm(), $_POST['am'] );
		
		$this->tkeys['CAL_EVENT_ID'] = $_POST['event_id'];	
                $this->tkeys['upload_max_filesize'] = ini_get('upload_max_filesize');
                $this->tkeys['post_max_size'] = ini_get('post_max_size');
                $this->tkeys['max_execution_time'] = ini_get('max_execution_time')/60;
		$this->tkeys['cal_linked_file'] = $_POST['linked_file_1'];
                $this->tkeys['cal_mime_type'] = $this->make_select_form('mime_type', $this->types_array, $_POST['mime_type']);
                include_once(SF_CACHE_PATH.'/language_options.inc');
                $language_options[''] = ' '.$tr->trans('select_a_language');
                asort($language_options);
                $this->tkeys['local_language'] = $this->make_select_form('language_id', $language_options, $_POST['language_id']);
                $text_options = array(
                    "t" => "text/plain",
                    "h" => "text/html"
                );
                $this->tkeys['select_artmime'] = $this->make_select_form('artmime', $text_options, $_POST['artmime']);
	}	
	
	
	function UpdateEventtoDB ($tr)
	{
	
		$event = new Event();
		$event = $event->find_by_id($_POST['event_id']);
		
		if ( 
			 $event!=null && 
			 settype( $_POST['confirmation_number'], "integer") && 
			 ( strlen ( trim ( $_POST['title'] ) ) > 0 ) &&
			 ( $_POST['duration'] > 0 ) 
		   )
		{
			$this->cleanup_post();
			$event->set_title ( $_POST['title'] );
			$event->set_start_day ( $_POST['start_day'] );
			$event->set_start_month ( $_POST['start_month'] );
			$event->set_start_year ( $_POST['start_year'] );

			$start_time = $_POST['start_hour'] + $_POST['am'] + ( $_POST['start_minute'] / 60 );
			
			if ($_POST['start_hour'] == 12)
			{
				$start_time=$start_time-12;
			}
			
			$event->set_start_time ($start_time);
			$event->set_duration ($_POST['duration']);
			$event->set_location_id ($_POST['location_id']);
			$event->set_location_other ($_POST['location_other']);
			$event->set_location_details ($_POST['location_details']);
			$event->set_event_type_id ($_POST['event_type_id']);
			$event->set_event_type_other ($_POST['event_type_other']);
			$event->set_event_topic_id ($_POST['event_topic_id']);
			$event->set_event_topic_other ($_POST['event_topic_other']);
			$event->set_contact_name ($_POST['contact_name']);
			$event->set_contact_phone ( $_POST['contact_phone'] );
			$event->set_contact_email ( $_POST['contact_email'] );
			$event->set_description ( $_POST['description'] );
			$event->set_file($_POST['linked_file_1']);
			$event->set_mime_type($_POST['mime_type']);
			$event->set_language_id($_POST['language_id']);
			$event->set_artmime($_POST['artmime']);
			
			$error_num = $event->update ( $_POST['confirmation_number'] );

			if ( $error_num < 0 )
			{
				$this->tkeys['CAL_ERROR_MSG'] = $tr->trans('cal_error_event_update').' '.$_POST['confirmation_number'].' - '.$error_num;
				$this->GetEventDatafromPOST ($tr);
				
			} else {

				$event->createWeekCache ( $event->get_start_date_object() );
				$event->createEventCache ( $event->get_id() );		
				$event->make_calendar_minical();
				$event->make_calendar_minical($_POST['location_id']);
				$event->make_calendar_rdf();
                                $event->make_calendar_rdf($_POST['location_id']);
				header("Location: event_display_week.php");
				
				exit;
			}
			
		} else {

			$this->tkeys['CAL_ERROR_MSG'] = $tr->trans('cal_error_event_update_title');
			$this->GetEventDatafromPOST ($tr);
		}
	
	
	}
	
	function translation ($tr) {
		$this->tkeys['CAL_LANG_SITE_CRUMB'] = $GLOBALS['site_crumb'];
		$this->tkeys['CAL_LANG_RETURN_TO_CALENDAR'] = $tr->trans('cal_return_to_calendar');
		$this->tkeys['CAL_LANG_CONF_NO'] = $tr->trans('cal_conf_no');
		$this->tkeys['CAL_LANG_TITLE'] = $tr->trans('cal_title');
		$this->tkeys['CAL_LANG_START_DATE'] = $tr->trans('cal_start_date');
		$this->tkeys['CAL_LANG_START_TIME'] = $tr->trans('cal_start_time');
		$this->tkeys['CAL_LANG_DURATION'] = $tr->trans('cal_duration');
		$this->tkeys['CAL_LANG_HOURS'] = $tr->trans('cal_hours');
		$this->tkeys['CAL_LANG_LLOCATION'] = $tr->trans('cal_location');
		$this->tkeys['CAL_LANG_LOCATION_DETAILS'] = $tr->trans('cal_location_details');
		$this->tkeys['CAL_LANG_OTHER'] = $tr->trans('cal_other');
		$this->tkeys['CAL_LANG_EVENT_TYPE'] = $tr->trans('cal_event_type');
		$this->tkeys['CAL_LANG_EVENT_TOPIC'] = $tr->trans('cal_event_topic');
		$this->tkeys['CAL_LANG_CONTACT_NAME'] = $tr->trans('cal_contact_name');
		$this->tkeys['CAL_LANG_CONTACT_EMAIL'] = $tr->trans('cal_contact_email');
		$this->tkeys['CAL_LANG_CONTACT_PHONE'] = $tr->trans('cal_contact_phone');
		$this->tkeys['CAL_LANG_DESCRIPTION'] = $tr->trans('cal_description');
	}

}
?>
