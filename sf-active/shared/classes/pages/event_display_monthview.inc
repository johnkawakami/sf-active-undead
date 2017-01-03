<?php

// Class for display_calendar_list

include_once(SF_SHARED_PATH . '/classes/pages/archive_display_calendar.inc');
include_once(SF_SHARED_PATH . '/classes/date_class.inc');

class event_display_monthview extends archive_display_calendar
{
    function event_display_monthview ()
    {
	return 1;
    }
    
    var $earliest_year;
    var $earliest_month;
    var $year;
    var $month;
    var $date;
    var $category_fields_loaded = true ;
    
   function get_cachefile_prefix(){
        return SF_CACHE_PATH."/calendar/event_week_cache";
    }

    function get_linkfile_prefix(){
    	return SF_CALENDAR_URL."/event_display_week.php?";
    }

    function get_cachefile_suffix(){
    	return "";
    }

    function setup_date(){
	$this->date= new Date;
    	$this->earliest_year=$GLOBALS['site_start_year'];
    	$this->earliest_month = $GLOBALS['site_start_month'];
        $this->date->set_date_to_now();
        $this->year = $this->date->get_year();
        $this->month = $this->date->get_month()+3;
    }


}

?>
