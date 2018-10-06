<?php

// Class for display_calendar_list

include_once(SF_SHARED_PATH . '/classes/date_class.inc');

class archive_display_calendar extends Page
{

    function archive_display_calendar()
    {
	$category_fields_loaded=false;
        return 1;
    }

    var $earliest_year;
    var $earliest_month;
    var $year;
    var $month;
    var $date;
    var $category_fields;
    var $category_fields_loaded;
    
    function setup_date(){
	$this->date= new Date;
    	$this->earliest_year=$GLOBALS['site_start_year'];
    	$this->earliest_month = $GLOBALS['site_start_month'];
        $this->date->set_date_to_now();
        $this->year = $this->date->get_year();
        $this->month = $this->date->get_month();
    }

    function get_cachefile_prefix(){
	if ((strlen($_GET['category_id'])>0) && ($_GET['category_id'] != $GLOBALS['config_defcategory']))
            {
		if (!$this->category_fields_loaded){
            $category_obj= new Category();
            $this->category_fields_loaded=true;
			$this->category_fields=$category_obj->get_category_fields($_GET['category_id']);
		}
		//echo $GLOBALS['archive_cache_path']."/".$this->category_fields[shortname];
                return $GLOBALS['archive_cache_path']."/".$this->category_fields['shortname'];
            }
	else{
		return $GLOBALS['archive_cache_path'];
	}
    }

    function get_linkfile_prefix(){
    	return SF_ARCHIVE_URL."/archive_week_redirect.php?category_id=".$_GET['category_id']."&amp;";
    }

    function get_cachefile_suffix(){
    	return ".php";
    }


    function execute()
    {
	$this->setup_date();

        $tblhtml = "";
        include_once(SF_SHARED_PATH . '/classes/calendar/calendar.inc');
        include_once(SF_SHARED_PATH . '/classes/calendar/archive_calendar.inc');


        $db_obj = new SFACTIVE\DB;
        $category_obj = new SFACTIVE\Category();
		$tr = new Translate ;
        if (strlen($_GET['category_id']) < 1)
        {
            $category_id = $GLOBALS['config_defcategory'];
        } else
        {
            $category_id = $_GET['category_id'];
        }

        if (!$this->category_fields_loaded) {
            $category_fields = $category_obj->get_category_fields($category_id);
            $this->category_fields_loaded=true;
        }
        $GLOBALS['page_title'] = $category_fields['name'] . " " . $tr->trans('archive_week');

        $calendar = new ArchiveCalendar();

	$date= new Date;
        $date->set_date_to_now();

        while ($this->year >= $this->earliest_year)
        {
            $tblhtml .= "<td colspan=\"3\"><big><b>".$this->year."</b></big></td></tr><tr>";

            $imonth = 12;
            $calendar->calendar_base_link_web_path = $this->get_linkfile_prefix();
            $calendar->calendar_cachefile_basepath= $this->get_cachefile_prefix();
            $calendar->calendar_cachefile_suffix=$this->get_cachefile_suffix();

            while ($this->month > 0)
            {
                if ($this->year == $this->earliest_year && $this->month < $this->earliest_month)
                {
                    break;
                }

                if ((($imonth %  3)==0) && ($this->month !=12))
                {
                    $tblhtml .= "</tr><tr>";
                }

                $tblhtml .= "<td valign=\"top\"> ";
		
                $tblhtml .= $calendar->getMonthView($this->month, $this->year, $this->date);
                $tblhtml .= "</td>";

                $this->month = $this->month-1;
                $imonth = $imonth-1;
            }

            $tblhtml .= "</tr><tr>";

            $this->month=12;
            $imonth = $this->month;
            $this->year = $this->year-1;
        }
        
        $this->tkeys['local_cat_id'] = $category_id;
        $this->tkeys['local_rows'] = str_replace("<tr></tr>", " ", $tblhtml);

    $this->tkeys['cat_shortname']=$category_fields['shortname'];
    $this->tkeys['cat_name']=$category_fields['name'];
    $this->tkeys['feature_url']=SF_FEATURE_URL.'/'.$this->tkeys['cat_shortname'].'/';
    if ($category_id==$GLOBALS['config_defcategory'])
    {
        $this->tkeys['feature_url']="/";
    }   

        return 1;
    }
}

?>
