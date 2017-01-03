<?php
//This file displays a list of events for printing


class event_display_list extends Page {

    function event_display_list () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () 
	{

		include SF_CALENDAR_PATH."/common_include.inc";		

		// Translate all TPL_ in the Template
		$tr = new Translate();
		$tr->create_translate_table('event_display_list');
		$this->translation($tr);

		$location = new Location();
		$event_topic = new EventTopic();
		$event_type = new EventType();
		$location = new Location();
		$event_renderer = new EventRenderer();
		$date_renderer = new DateRenderer();

		$Title="Event Listing";

		if (strlen($_GET['topic_id'])>0) $_POST['topic_id']=$_GET['topic_id'];

		if ( strlen ($_POST['topic_id'] ) > 0 )
		{
			$filter[topic_id] = $_POST['topic_id'];
		}

		if (strlen($_GET['type_id'])>0) $_POST['type_id']=$_GET['type_id'];

		if ( strlen($_POST['type_id']) > 0 ) 
		{
			$filter[type_id] = $_POST['type_id'];
		}

		if (strlen($_GET['location_id'])>0) $_POST['location_id']=$_GET['location_id'];

		if ( strlen($_POST['location_id']) > 0 )
		{
			$filter[location_id] = $_POST['location_id'];
		}

		if ( strlen($_POST['begin_month']) > 0 )
		{
			$begin_date = new Date;
			$begin_date->set_time ( 0,0,$_POST['begin_day'], $_POST['begin_month'], $_POST['begin_year'] );

			$end_date = new Date;
			$end_date->set_time ( 0,0,$_POST['end_day'],$_POST['end_month'],$_POST['end_year'] );

		} else {

			$begin_date = new Date;
			$begin_date->set_date_to_now();
			$begin_date->move_to_start_of_day();
			$end_date = new Date;
			$end_date->set_date_to_now();
			$end_date->move_forward_n_days(14);

			$begin_month = $begin_date->get_month();
                        $_POST['begin_month'] = $begin_month;
			$begin_day = $begin_date->get_day();
                        $_POST['begin_day'] = $begin_day;
			$begin_year = $begin_date->get_year();
                        $_POST['begin_year'] = $begin_year;
			$end_month = $end_date->get_month();
                        $_POST['end_month'] = $end_month;
			$end_day = $end_date->get_day();
                        $_POST['end_day'] = $end_day;
			$end_year = $end_date->get_year();
                        $_POST['end_year'] = $end_year;
		}

                if (strlen($_GET['detail_flag'])>0) $_POST['detail_flag']=$_GET['detail_flag'];
		if ( strlen($_POST['detail_flag']) == 0 )
		{
			$this->tkeys['CAL_DF_SELECT'] = "";
			$detail_flag="off";
		} else {
			$this->tkeys['CAL_DF_SELECT'] = 'checked="checked"';
			$detail_flag="on";
		}
                $this->tkeys['CAL_DF'] = true;
		
		// we don't need dbase any longer for this.
		//$this->tkeys['CAL_DD_EVENT_TOPIC'] = $event_renderer->create_dropdown($event_topic->find_all(), $_POST['topic_id'] );
		//$this->tkeys['CAL_DD_EVENT_TYPE'] = $event_renderer->create_dropdown($event_type->find_all(), $_POST['type_id'] );
		//$this->tkeys['CAL_DD_LOCATION'] = $event_renderer->create_dropdown($location->find_all(), $_POST['location_id'] );
		$event_topic_options[''] = $tr->trans('cal_all');
		$event_type_options[''] = $tr->trans('cal_all');
		$location_options[''] = $tr->trans('cal_all');
		$this->tkeys['cal_dd_event_topic'] = $this->make_select_form('topic_id', $event_topic_options, $_POST['topic_id']);
		$this->tkeys['cal_dd_event_type'] = $this->make_select_form('type_id', $event_type_options, $_POST['type_id']);
		$this->tkeys['cal_dd_location'] = $this->make_select_form('location_id', $location_options, $_POST['location_id']);
		$this->tkeys['CAL_DD_START_MONTH'] = $event_renderer->create_dropdown($date_renderer->get_months(),$_POST['begin_month'] );
		$this->tkeys['CAL_DD_START_DAY'] = $event_renderer->create_dropdown($date_renderer->get_days_of_month(), $_POST['begin_day'] );
		$this->tkeys['CAL_DD_START_YEAR'] =	$event_renderer->create_dropdown($date_renderer->get_years(), $_POST['begin_year'] );
		$this->tkeys['CAL_DD_MONTH_END'] = $event_renderer->create_dropdown($date_renderer->get_months(),$_POST['end_month'] );
		$this->tkeys['CAL_DD_DAY_END'] = $event_renderer->create_dropdown($date_renderer->get_days_of_month(), $_POST['end_day'] );
		$this->tkeys['CAL_DD_YEAR_END'] = $event_renderer->create_dropdown($date_renderer->get_years(), $_POST['end_year'] );


		$event = new Event();

		$begin_date = $begin_date->get_unix_time();
		$end_date = $end_date->get_unix_time();

		$event_array = $event->find_between_dates(
			date("Y/m/d",$begin_date),
			date("Y-m-d",$end_date),
			$filter);

		$event_array = array_reverse ($event_array);

		$this->tkeys['CAL_EVENT_LIST'] = $event_renderer->render_list($event_array,$detail_flag);

	}

	function translation ($tr) {
	
		$this->tkeys['CAL_LANG_TOPIC'] = $tr->trans('cal_topic');
		$this->tkeys['CAL_LANG_ALL'] = $tr->trans('cal_all');
		$this->tkeys['CAL_LANG_TYPE'] = $tr->trans('cal_type');
		$this->tkeys['CAL_LANG_LOCATION'] = $tr->trans('cal_location');
		$this->tkeys['CAL_LANG_START_DATE'] = $tr->trans('cal_start_date');
		$this->tkeys['CAL_LANG_END_DATE'] = $tr->trans('cal_end_date');
		$this->tkeys['CAL_LANG_DISPLAY_DETAILS'] = $tr->trans('cal_display_details');
		$this->tkeys['CAL_LANG_VIEW_EVENTS'] = $tr->trans('cal_view_events');
	
	}
	
}
