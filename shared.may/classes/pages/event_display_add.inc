<?php // vim:et:ai:ts=4:sw=4

// Class for the Calender Add and Update Form

class event_display_add extends Page {

    function event_display_add () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
	
		include SF_CALENDAR_PATH."/common_include.inc";

		// Translate all TPL_ in the Template
		$tr = new Translate();
		$tr->create_translate_table('event_display_add');		
		$tr->create_translate_table('publish');
		$this->translation($tr);

		$this->location_options = $location_options;
		$this->event_type_options = $event_type_options;
		$this->event_topic_options = $event_topic_options;

		if ( $_POST['caction']  == "" && strlen($_GET['event_id']) == 0 )
		{
			$this->SetNewEventDefaults($tr);
		}

		if ( $_POST['caction']  == "addnew" && strlen($_GET['event_id']) == 0 )
		{
			$this->AddEventtoDB($tr);
		}

		return 1;		
	}
	
	
	function SetNewEventDefaults ($tr) 
	{
        $GLOBALS['dropdown_min_year']=date('Y');
		// Init needed for DD generation
		$location = new \SFACTIVE\Calendar\Location();
		$event_topic = new \SFACTIVE\Calendar\EventTopic();
		$event_type = new \SFACTIVE\Calendar\EventType();
		$event_renderer = new \SFACTIVE\Calendar\EventRenderer();
		$date_renderer = new DateRenderer();
		
		if (!$_SESSION['craptcha']) $_SESSION['craptcha'] = rand(1,999999);
		$captcha = new Craptcha($_SESSION['craptcha']);
	
		// Use Today as Default Date
		$start_day = date("d",time());
		$start_month = date("m",time());
		$start_year = date("Y",time());

		// Set Form Action Stuff
		$this->tkeys['CAL_ACTION_TITLE'] = $tr->trans('cal_add');
		$this->tkeys['CAL_FORM_NAME'] = "event_add";
		$this->tkeys['CAL_FORM_FILE'] = "event_display_add.php";
		$this->tkeys['CAL_CACTION'] = "addnew";

		// Set all Templates to Zero

		$this->tkeys['CAL_PTITLE'] = "";
		$this->tkeys['CAL_PDURATION'] = "1";

		// Location
		$this->tkeys['CAL_DD_LOCATION_ID'] = $this->make_select_form('location_id', $this->location_options, 0 );
		$this->tkeys['CAL_LOCATION_OTHER'] = "";
		$this->tkeys['CAL_LOCATION_DETAILS'] = "";

		// Event Type
		$this->tkeys['CAL_DD_EVENT_TYPE_ID'] = $this->make_select_form('event_type_id', $this->event_type_options, 0 );
		$this->tkeys['CAL_EVENT_TYPE_OTHER'] = "";

		// Event Topic
		$this->tkeys['CAL_DD_EVENT_TOPIC_ID'] = $this->make_select_form('event_topic_id', $this->event_topic_options, 0 ); 
		$this->tkeys['CAL_EVENT_TOPIC_OTHER'] = "";


		// Contact
		$this->tkeys['CAL_CONTACT_NAME'] = "";
		$this->tkeys['CAL_CONTACT_PHONE'] = "";
		$this->tkeys['CAL_CONTACT_EMAIL'] = "";

		$this->tkeys['CAL_DISCRIPTION'] = "";


		$this->tkeys['CAL_DD_START_MONTH'] = $event_renderer->create_dropdown($date_renderer->get_months(),$start_month);			
		$this->tkeys['CAL_DD_START_DAY'] = $event_renderer->create_dropdown($date_renderer->get_days_of_month(),$start_day);
		$this->tkeys['CAL_DD_START_YEAR'] = $event_renderer->create_dropdown($date_renderer->get_years(),$start_year);

		$this->tkeys['CAL_DD_START_HOUR'] = $event_renderer->create_dropdown($date_renderer->get_hours(),12);
		$this->tkeys['CAL_DD_START_MINUTE'] = $event_renderer->create_dropdown($date_renderer->get_minutes(),00);
		$this->tkeys['CAL_DD_AMPM'] = $event_renderer->create_dropdown($date_renderer->get_ampm(), 12);

		$this->tkeys['CAL_CONF_NO'] = "";
		
		$this->tkeys['CAL_ERROR_MSG'] = "";
        $this->tkeys['upload_max_filesize'] = ini_get('upload_max_filesize');
		$this->tkeys['post_max_size'] = ini_get('post_max_size');
		$this->tkeys['max_execution_time'] = ini_get('max_execution_time')/60;
		include_once(SF_CACHE_PATH.'/language_options.inc');
		$language_options[''] = ' '.$tr->trans('select_a_language');
		asort($language_options);
		$this->tkeys['local_language'] = $this->make_select_form('language_id', $language_options, '');
	        $text_options = array(
		    "t" => "text/plain",
		    "h" => "text/html"
		);
		$this->tkeys['select_artmime'] = $this->make_select_form('artmime', $text_options, '');

		$this->tkeys['CAPTCHA'] = 'What is the total of <strong>'.$captcha->toString().'</strong>? (spell answer out in English)<input type="text" size="8" name="captcha" />';
	}

	function GetEventDatafromPOST ($tr) 
	{
        // Get the Post Data Back to the Form if there is a Error	
        $GLOBALS['dropdown_min_year']=date('Y');
	
		// We need that to create the drop downs
		$event = new \SFACTIVE\Calendar\Event();
		
		$location = new \SFACTIVE\Calendar\Location();
		$event_topic = new \SFACTIVE\Calendar\EventTopic();
		$event_type = new \SFACTIVE\Calendar\EventType();
		$event_renderer = new \SFACTIVE\Calendar\EventRenderer();
		$date_renderer = new DateRenderer();

	
		// Set Form Action Stuff
		if ($_POST['caction'] == "update") 
		{
			$this->tkeys['CAL_ACTION_TITLE'] = $tr->trans ('cal_update');
			$this->tkeys['CAL_FORM_NAME'] = "event_update";
			$this->tkeys['CAL_FORM_FILE'] = "event_display_add.php";
			$this->tkeys['CAL_CACTION'] = $_POST['caction'];

		} else if ($_POST['caction'] == "addnew") {

			$this->tkeys['CAL_ACTION_TITLE'] = $tr->trans ('cal_add');
			$this->tkeys['CAL_FORM_NAME'] = "event_add";
			$this->tkeys['CAL_FORM_FILE'] = "event_display_add.php";
			$this->tkeys['CAL_CACTION'] = $_POST['caction'];

		}

		$this->tkeys['CAL_PTITLE'] = $_POST['title'];
		$this->tkeys['CAL_PDURATION'] = $_POST['duration'];

		$this->tkeys['CAL_DD_LOCATION_ID'] = $this->make_select_form('location_id',  $this->location_options, $_POST['location_id'] );
		$this->tkeys['CAL_LOCATION_OTHER'] = $_POST['location_other'];
		$this->tkeys['CAL_LOCATION_DETAILS'] = $_POST['location_details'];

		// Event Type
		$this->tkeys['CAL_DD_EVENT_TYPE_ID'] = $this->make_select_form('event_type_id', $this->event_type_options, $_POST['event_type_id'] );
		$this->tkeys['CAL_EVENT_TYPE_OTHER'] = $_POST['event_type_other'];

		// Event Topic
		$this->tkeys['CAL_DD_EVENT_TOPIC_ID'] = $this->make_select_form('event_topic_id', $this->event_topic_options, $_POST['event_topic_id'] );
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
        $this->tkeys['upload_max_filesize'] = ini_get('upload_max_filesize');
        $this->tkeys['post_max_size'] = ini_get('post_max_size');
        $this->tkeys['max_execution_time'] = ini_get('max_execution_time')/60;
        include_once(SF_CACHE_PATH.'/language_options.inc');
        $language_options[''] = ' '.$tr->trans('select_a_language');
        asort($language_options);
        $this->tkeys['local_language'] = $this->make_select_form('language_id', $language_options, $_POST['language_id']);
        $text_options = array(
            "t" => "text/plain",
            "h" => "text/html"
        );
        $this->tkeys['select_artmime'] = $this->make_select_form('artmime', $text_options, $_POST['artmime']);

		if (!$_SESSION['craptcha']) $_SESSION['craptcha'] = rand(1,999999);
		$captcha = new Craptcha($_SESSION['craptcha']);
		$this->tkeys['CAPTCHA'] = 'What is the total of <strong>'.$captcha->toString().'</strong>? (spell answer out in English)<input type="text" size="8" name="captcha" />';
	}
	
	function AddEventtoDB ($tr) {
		if (!$_SESSION['craptcha']) $_SESSION['craptcha'] = rand(1,999999);
		$captcha = new Craptcha($_SESSION['craptcha']);
		if ( 
		     ( strlen (trim($_POST['title'])) <= 0 ) && 
			 ( $_POST['duration'] > 0 ) &&
			 ( $captcha->answerMatches($_POST['captcha']) )
		   )
		{
			$this->tkeys['CAL_ERROR_MSG'] = $tr->trans('cal_error_event_add');
			$this->GetEventDatafromPOST ($tr);
		} else {
		    $event = new \SFACTIVE\Calendar\Event();
		    if(strlen($_FILES['linked_file_1']['name']) > 0) $_POST['file_count'] = 1;
		    $event->article['created_month'] = $_POST['start_month'];
		    $event->article['created_year'] = $_POST['start_year'];
		    if($event->article['created_month'] < 10) $event->article['created_month'] = "0".$event->article['created_month'];
			if (!$captcha->answerMatches($_POST['captcha']))
			{
			    $this->tkeys['CAL_ERROR_MSG'] = '<ul><li>You didn\'t answer the math question correctly.</li></ul>';
			    $this->GetEventDatafromPOST ($tr);
			} else if(!$event->validate_event_post()) {
			    $this->tkeys['CAL_ERROR_MSG'] = $event->get_error_status() ;
			    $this->GetEventDatafromPOST ($tr);
		    } else {
				$captcha->clear();
				$event->cleanup_post();
				$event->process_uploads();
				// $_POST['description'] = mysql_escape_string($_POST['description']);
				// $_POST['location_details'] = mysql_escape_string($_POST['location_details']);
				$event->set_title($_POST['title']);
				$event->set_start_day($_POST['start_day']);
				$event->set_start_month($_POST['start_month']);
				$event->set_start_year($_POST['start_year']);
				$event->set_file($event->upload_target_url[1]);
				$event->set_mime_type($_POST['mime_type_file_1']);
				$start_time = $_POST['start_hour'] + $_POST['am'] + ($_POST['start_minute'] / 60);

				if ( $_POST['start_hour'] == 12 )
				{
					$start_time=$start_time-12;
				}
				
				$event->set_start_time($start_time);
				$event->set_duration($_POST['duration']);
				$event->set_location_id($_POST['location_id']);
				$event->set_location_other($_POST['location_other']);
				$event->set_location_details($_POST['location_details']);
				$event->set_event_topic_id($_POST['event_topic_id']);
				$event->set_event_topic_other($_POST['event_topic_other']);
				$event->set_event_type_id($_POST['event_type_id']);
				$event->set_event_type_other($_POST['event_type_other']);
				$event->set_contact_name($_POST['contact_name']);
				$event->set_contact_email($_POST['contact_email']);
				$event->set_contact_phone($_POST['contact_phone']);
				$event->set_description($_POST['description']);
				$event->set_language_id($_POST['language_id']);
				$event->set_artmime($_POST['artmime']);
				
				$confirmation_number = $event->add();
				
				if ($confirmation_number > 0 ) 
				{
					$event->createWeekCache ( $event->get_start_date_object() );
					$event->createEventCache ( $event->get_id() );
					$event->make_calendar_minical();
					$event->make_calendar_minical($_POST['location_id']);
					$event->make_calendar_rdf();
									$event->make_calendar_rdf($_POST['location_id']);

					header("Location: event_display_add_confirm.php?confirmation_number=".$confirmation_number);

				} else {

					header("Location: event_display_add_confirm.php?confirmation_number=".$confirmation_number);
					
				}
			}
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
