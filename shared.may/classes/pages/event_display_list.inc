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

		$location = new \SFACTIVE\Calendar\Location();
		$event_topic = new \SFACTIVE\Calendar\EventTopic();
		$event_type = new \SFACTIVE\Calendar\EventType();
		$location = new \SFACTIVE\Calendar\Location();
		$event_renderer = new \SFACTIVE\Calendar\EventRenderer();
		$date_renderer = new DateRenderer();

		$Title="Event Listing";


		$filter['topic_id'] = filter_input( INPUT_POST, 'topic_id', FILTER_VALIDATE_INT );
		$filter['type_id'] = filter_input( INPUT_POST, 'type_id', FILTER_VALIDATE_INT );
		$filter['location_id'] = filter_input( INPUT_POST, 'location_id', FILTER_VALIDATE_INT );

		$begin_day = filter_input( INPUT_POST, 'begin_day', FILTER_VALIDATE_INT );
		$begin_month = filter_input( INPUT_POST, 'begin_month', FILTER_VALIDATE_INT );
		$begin_year = filter_input( INPUT_POST, 'begin_year', FILTER_VALIDATE_INT );
		$end_day = filter_input( INPUT_POST, 'end_day', FILTER_VALIDATE_INT );
		$end_month = filter_input( INPUT_POST, 'end_month', FILTER_VALIDATE_INT );
		$end_year = filter_input( INPUT_POST, 'end_year', FILTER_VALIDATE_INT );

		if ( $begin_year || $begin_month || $begin_day )
		{
			$begin_date = new Date;
			$begin_date->set_time ( 0, 0, $begin_day, $begin_month, $begin_year );

			$end_date = new Date;
			$end_date->set_time ( 0, 0, $end_day, $end_month, $end_year );

		} else {

			$begin_date = new Date;
			$begin_date->set_date_to_now();
			$begin_date->move_to_start_of_day();
			$end_date = new Date;
			$end_date->set_date_to_now();
			$end_date->move_forward_n_days(14);

			$begin_month = $begin_date->get_month();
			$begin_day = $begin_date->get_day();
			$begin_year = $begin_date->get_year();
			$end_month = $end_date->get_month();
			$end_day = $end_date->get_day();
			$end_year = $end_date->get_year();
		}

		$detail_flag = filter_input( INPUT_POST, 'detail_flag', FILTER_VALIDATE_INT );
		if ( strlen($detail_flag) == 0 )
		{
			$this->tkeys['CAL_DF_SELECT'] = "";
			$detail_flag="off";
		} else {
			$this->tkeys['CAL_DF_SELECT'] = ' checked="checked"';
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
		$this->tkeys['cal_dd_event_topic'] = $this->make_select_form('topic_id', $event_topic_options, $topic_id);
		$this->tkeys['cal_dd_event_type'] = $this->make_select_form('type_id', $event_type_options, $type_id);
		$this->tkeys['cal_dd_location'] = $this->make_select_form('location_id', $location_options, $location_id);
		$this->tkeys['CAL_DD_START_MONTH'] = $event_renderer->create_dropdown($date_renderer->get_months(),$begin_month );
		$this->tkeys['CAL_DD_START_DAY'] = $event_renderer->create_dropdown($date_renderer->get_days_of_month(), $begin_day );
		$this->tkeys['CAL_DD_START_YEAR'] =	$event_renderer->create_dropdown($date_renderer->get_years(), $begin_year );
		$this->tkeys['CAL_DD_MONTH_END'] = $event_renderer->create_dropdown($date_renderer->get_months(),$end_month );
		$this->tkeys['CAL_DD_DAY_END'] = $event_renderer->create_dropdown($date_renderer->get_days_of_month(), $end_day );
		$this->tkeys['CAL_DD_YEAR_END'] = $event_renderer->create_dropdown($date_renderer->get_years(), $end_year );


		$event = new \SFACTIVE\Calendar\Event();

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
