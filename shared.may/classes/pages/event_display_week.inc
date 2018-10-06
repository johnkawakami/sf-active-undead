<?php // vim:et:ai:ts=4:sw=4

// Class for the Calender Main Page...

class event_display_week extends Page {

    function event_display_week () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
        include SF_CALENDAR_PATH."/common_include.inc";     
        
        // Translate all TPL_ in the Template
        $tr = new Translate();
        $tr->create_translate_table('event_display_week');      
        $this->translation($tr);

        $event_topic = new \SFACTIVE\Calendar\EventTopic();
        // added to have filter by location
        $event_location = new \SFACTIVE\Calendar\Location();
        $event_renderer = new \SFACTIVE\Calendar\EventRenderer();
        $event_display = new \SFACTIVE\Calendar\EventDisplay();    // Event Display Helper Class
        $date = new Date;
        
        // Topic ID
        $topicId = filter_input( INPUT_GET, 'event_topic', FILTER_VALIDATE_INT);
        $locationId = filter_input( INPUT_GET, 'event_location', FILTER_VALIDATE_INT);
        $hasFilters = ( ($topicId !== null) || ($locationId !== null) );

        $this->tkeys['CAL_ID_TOPIC'] = ($topicId===false ? '' : $topicId);
        $this->tkeys['CAL_ID_LOCATION'] = ($locationId===false ? '': $locationId);
        
        // Naviagtion 
        $day = filter_input( INPUT_GET, 'day', FILTER_VALIDATE_INT);
        $month = filter_input( INPUT_GET, 'month', FILTER_VALIDATE_INT);
        $year = filter_input( INPUT_GET, 'year', FILTER_VALIDATE_INT);

        if ($day || $month || $year) {
            $date->set_time( 0, 0, $day, $month, $year );
        } else {
            $date->set_date_to_now();
        }
        $datenow = $date->aclone();
        $datenow->find_start_of_week();
        $date->find_start_of_week();

        $nextweek = $date->aclone();
        if ($nextweek->get_year() < (date('Y')+3)) $nextweek->move_n_weeks_forward(1);
        $nextweek_day = $nextweek->get_day();
        $nextweek_month = $nextweek->get_month();
        $nextweek_year = $nextweek->get_year();

        $this->tkeys['CAL_NEXT_WD'] = $nextweek_day;
        $this->tkeys['CAL_NEXT_WM'] = $nextweek_month;
        $this->tkeys['CAL_NEXT_WY'] = $nextweek_year;       
        
        $lastweek = $date->aclone();
        if ($lastweek->get_year() > $GLOBALS['site_start_year']-1) $lastweek->move_n_weeks_forward(-1);
        $lastweek_day = $lastweek->get_day();
        $lastweek_month = $lastweek->get_month();
        $lastweek_year = $lastweek->get_year();

        $this->tkeys['CAL_LAST_WD'] = $lastweek_day;
        $this->tkeys['CAL_LAST_WM'] = $lastweek_month;
        $this->tkeys['CAL_LAST_WY'] = $lastweek_year;
        
        // Short Month View
        $this->tkeys['CAL_EVENT_MONTH_VIEW_PREV'] = $event_display->renderShortMonthViewPrev($date, $datenow);
        $this->tkeys['CAL_EVENT_MONTH_VIEW_NEXT'] = $event_display->renderShortMonthViewNext($date, $datenow);

        // Topic Drop Down
        $event_topic_options[''] = $tr->trans('all');
        $location_options[''] = $tr->trans('all');
        $this->tkeys['CAL_EVENT_LOCATION_DROPDOWN'] = $this->make_select_form('event_location', $location_options, $locationId);
        $this->tkeys['cal_event_topic_dropdown'] = $this->make_select_form('event_topic', $event_topic_options, $topicId);
        
        $this->tkeys['CAL_CUR_DATE'] = date("m/d/Y",$date->get_unix_time());

        $this->tkeys['CAL_DAY'] = $day;
        $this->tkeys['CAL_MONTH'] = $month;
        $this->tkeys['CAL_YEAR'] = $year;

        // Daytitles
        $tmpday = 0;
        while ( $tmpday < 7 )
        {
            $datytitles .='<td nowrap="nowrap" class="weekTitles">';
            $datytitles .= $event_renderer->render_daytitle($date);
            $datytitles .='</td>';
            $date->move_forward_n_days(1);
            $tmpday++;
        }
        
        $this->tkeys['CAL_EVENT_MONTH_DAYTITLE'] = $datytitles;

        // Event Week Render
        $date->move_forward_n_days(-7);
            
        $newdate = $date->aclone();
        $newdate->find_start_of_week();


        // Week View
        if ( $hasFilters ) {
            // We need DB if we have a Filter
            $this->tkeys['CAL_EVENT_MONTH_VIEW_FULL'] = $event_display->renderWeekHTML(
                $newdate, 
                array('topic_id' => $topicId, 'location_id' => $locationId)
            );

        } else {
            // No Filter, try to use the Cache Week File
            // $ccm = $GLOBALS['calendar_cache_manager'];
            $ccm = new \SFACTIVE\Calendar\CalendarCacheManager(SF_CACHE_PATH.'/calendar/');
            $this->tkeys['CAL_EVENT_MONTH_VIEW_FULL'] = $ccm->get_week_html($newdate);
        }
// here we create the searchform.
    $str = new Translate;
    $str->create_translate_table('newswire');
    $str->create_translate_table('calendar');
        $event_renderer = new \SFACTIVE\Calendar\EventRenderer();
        $date_renderer = new DateRenderer();
                $location_obj = new \SFACTIVE\Calendar\Location();
                $eventtopic_obj = new \SFACTIVE\Calendar\EventTopic();
                $eventtype_obj = new \SFACTIVE\Calendar\EventType();
                $tpl = new FastTemplate(SF_TEMPLATE_PATH."/pages/");
                $tpl->clear_all();
                $tpl->define(array('search' => 'calendar_search.tpl'));
                if ($author)
        {
            $search_vars["TPL_SEARCH_AUTHOR"]=$author;
        } else {
            $search_vars["TPL_SEARCH_AUTHOR"]=$tr->trans('author');
        }

        if ($keyword)
        {
            $search_vars["TPL_SEARCH_KEYWORD"]=$keyword;
        } else {
            $search_vars["TPL_SEARCH_KEYWORD"]=$str->trans('keyword');
        }

                $search_vars['TPL_AUTHOR'] = $tr->trans('author');
                $search_vars['TPL_KEYWORD'] = $str->trans('keyword');
                $search_vars['TPL_NO_SUMMARIES'] = $str->trans('no_summaries');
                $search_vars['TPL_GO'] = $str->trans('go');

                $search_vars["TPL_SEARCH_YEAR"] = "<select name=\"year\">\n";
                $search_vars["TPL_SEARCH_YEAR"] .='<option value="">'.$tr->trans('year').'</option>';
                $search_vars["TPL_SEARCH_YEAR"] .= $event_renderer->create_dropdown($date_renderer->get_years(),$year);
                $search_vars["TPL_SEARCH_YEAR"] .= "</select>";

                $search_vars["TPL_SEARCH_MONTH"] = "<select name=\"month\">\n";
                $search_vars["TPL_SEARCH_MONTH"] .='<Option value="">'.$tr->trans('month').'</option>';
                $search_vars["TPL_SEARCH_MONTH"] .= $event_renderer->create_dropdown($date_renderer->get_months(),$month);
                $search_vars["TPL_SEARCH_MONTH"] .= "</Select>";

                $search_vars["TPL_SEARCH_DAY"] = "<Select name=\"day\">\n";
                $search_vars["TPL_SEARCH_DAY"] .='<option value="">'.$tr->trans('day')."</option>";
        $search_vars["TPL_SEARCH_DAY"] .= $event_renderer->create_dropdown($date_renderer->get_days_of_month(),$day);
        $search_vars["TPL_SEARCH_DAY"] .= "</select>";

        
                // Let's move over to a static variant.
                // $search_vars["TPL_SEARCH_LOCATION"] = "<select name=\"location\"><option value=\"\">".$tr->trans('locations')."</option>". 
                // $event_renderer->create_dropdown($location_obj->find_all(), $location )."</select>";
                $location_options[''] = $tr->trans('locations');
                $search_vars['TPL_SEARCH_LOCATION'] = $this->make_select_form('location', $location_options,'');
                
                //$search_vars["TPL_SEARCH_EVENTTYPE"] = "<select name=\"eventtype\"><option value=\"\">".$tr->trans('event_types')."</option>". 
                //$event_renderer->create_dropdown($eventtype_obj->find_all(), $eventtype )."</select>";
                $event_type_options[''] = $tr->trans('event_types');
                $search_vars['TPL_SEARCH_EVENTTYPE'] = $this->make_select_form('eventtype', $event_type_options, '');
                
                //$search_vars["TPL_SEARCH_EVENTTOPIC"] = "<select name=\"eventtopic\"><option value=\"\">".$tr->trans('event_topics')."</option>". 
                //$event_renderer->create_dropdown($eventtopic_obj->find_all(), $eventtopic )."</select>";
                $event_topic_options[''] = $tr->trans('event_topics');
                $search_vars['TPL_SEARCH_EVENTTOPIC'] = $this->make_select_form('eventtopic', $event_topic_options, '');
                

                 $sort_options = array(
                  "" =>  $str->trans('sortby'),
                  "selected" =>  $str->trans('newest'),
          "reverse" =>  $str->trans('reverse'), );
                $search_vars["TPL_SEARCH_SORT"]=$this->make_select_form("sort",$sort_options, $sort);

                $limit_options = array(
                  "10" =>  "10",
                  "20" =>  "20",
                          "40" =>  "40",
                  "100" =>  "100",);
          if($GLOBALS['default_search_size'] !==(10|20|40|100))
                  {
                          $x = $GLOBALS['default_search_size'];
                          $limit_options["$x"] = $x;
                          asort($limit_options);
                  }
                                  
        $search_vars["TPL_SEARCH_LIMIT"]= $this->make_select_form("limit", $limit_options, $limit);

                if ($summary == "no")
            $search_vars["TPL_SEARCH_SUMMARIES"]='checked="checked"';
            else
                        $search_vars["TPL_SEARCH_SUMMARIES"]="";

                $search_vars["TPL_FORM_NAME"] = "calendar_admin_top";

        $search_vars["TPL_ACTION"] = $GLOBALS['calendar_url'].'/event_search.php';

        include_once(SF_CACHE_PATH.'/language_options.inc');
        $language_options[''] = ' '.$tr->trans('select_a_language');
        asort($language_options);
        $search_vars['TPL_LANGUAGE_FORM'] = $this->make_select_form('language_id', $language_options, '');


                $tpl->assign($search_vars);
                $tpl->parse("MAIN", "search");
                $searchhtml = $tpl->fetch('MAIN');
                $this->tkeys['form_search_top'] = $searchhtml;

                $tpl = new FastTemplate(SF_TEMPLATE_PATH."/pages");
                $tpl->clear_all();
                $tpl->define(array("bottom" => "calendar_search.tpl"));
                $search_vars["TPL_FORM_NAME"] = "calendar_admin_bottom";
                $tpl->assign($search_vars);
                $tpl->parse("MAIN", "bottom");
                $searchhtml = $tpl->fetch('MAIN');
                $this->tkeys['form_search_bottom'] = $searchhtml;

        return 1;       
    }


    function translation ($tr) {
        $this->tkeys['CAL_LANG_SITE_CRUMB'] = $GLOBALS['site_crumb'];
        $this->tkeys['CAL_LANG_CALENDAR'] = $tr->trans('cal_calendar');
        $this->tkeys['CAL_LANG_ALL'] = $tr->trans('all');
        $this->tkeys['CAL_LANG_PREV_WEEK'] = $tr->trans('cal_previous_week');
        $this->tkeys['CAL_LANG_NEXT_WEEK'] = $tr->trans('cal_next_week');
        $this->tkeys['CAL_LANG_WEEK_OFF'] = $tr->trans('cal_week_of');
        $this->tkeys['CAL_LANG_ADD_AN_EVENT'] = $tr->trans('cal_add_an_event');
        $this->tkeys['CAL_LANG_VIEW_ALL_MONTHS'] = $tr->trans('cal_view_all_months');
        $this->tkeys['CAL_LANG_LIST_FUTURE_EVENTS'] = $tr->trans('cal_list_future_events');
        $this->tkeys['CAL_LANG_FILTER_BY_TOPIC'] = $tr->trans('cal_filter_by_topic');
        // added to have filter by location
        $this->tkeys['CAL_LANG_FILTER_BY_LOCATION'] = $tr->trans('cal_filter_by_location');
        $this->tkeys['CAL_LANG_FILTER'] = $tr->trans('cal_filter');
    }
    
}
