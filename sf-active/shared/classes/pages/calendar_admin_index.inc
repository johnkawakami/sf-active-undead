<?php

// Class for calendar_admin_index page

class calendar_admin_index extends Page {

    function calendar_admin_index() {
        // Class constructor, does nothing
        return 1;
    }

    function execute() {
		// Execution method, does nothing
		include(SF_CALENDAR_PATH."/common_include.inc");
		global $time_diff;
        $tr = new Translate();
        $tr->create_translate_table('calendar');
		$str = new Translate ;
		$str->create_translate_table('newswire');
		$event_renderer = new EventRenderer();
        $date_renderer = new DateRenderer();

		$this->tkeys['event_id'] = $str->trans('go');


		//
		$author = $_GET['author'];
        $keyword = $_GET['keyword'];
        $page = $_GET['page'];
        if ($_GET['day'] and $_GET['day']<10) { $day = '0'.$_GET['day']; }
        else { $day = $_GET['day']; }
        if ($_GET['month'] and $_GET['month']<10) { $month = '0'.$_GET['month']; }
        else { $month = $_GET['month']; }
        $year = $_GET['year'];
        $display = $_GET['display'];
		$limit = $_GET['limit'];
		$summary = $_GET['summary'];
		$location = $_GET['location'];
		$eventtopic = $_GET['eventtopic'];
		$eventtype = $_GET['eventtype'];
		$sort = $_GET['sort'];
		$language_id = $_GET['language_id'];

        // the small navigation bar (<< 1 >>)
        if(!$limit) $limit = $GLOBALS['default_search_size'] ;
        if(!$page) { $page = 0; }
        $this_page = $page + 1;
        $prev = $page -1;
        $next = $page + 1;
        if($page > 0) { 
			$prev_page = "<a href=\"?page=$prev&amp;limit=$limit&amp;summary=$summary&amp;sort=$sort&amp;location=$location&amp;eventtype=$eventtype&amp;eventtopic=$eventtopic&amp;year=$year&amp;month=$month&amp;day=$day&amp;author=$author&amp;keyword=$keyword&amp;language_id=$language_id";
            $prev_page .= "\">&lt;&lt;</a>" ; 
		}else{ 
			$prev_page = ""; 
		}
		$next_page = "<a href=\"?page=$next&amp;limit=$limit&amp;summary=$summary&amp;sort=$sort&amp;location=$location&amp;eventtype=$eventtype&amp;eventtopic=$eventtopic&amp;year=$year&amp;month=$month&amp;day=$day&amp;author=$author&amp;keyword=$keyword&amp;language_id=$language_id";
        $next_page .= "\">&gt;&gt;</a>";
        $pager = $page * $limit ;
		$this->tkeys['nav'] = "<div>$prev_page $this_page $next_page</div>";

		// sql modifiers
		if($summary =="no") { $description = '';}
		else{ $description = " e.description,";}

		if($sort =="reverse") $sort_sql = 'asc';
		else $sort_sql = 'desc';

		if($location) $location_sql = " and e.location_id = '".$location."'";
		if($eventtype) $location_sql = " and e.event_type_id = '".$eventtype."'";
		if($eventtopic) $location_sql = " and e.event_topic_id = '".$eventtopic."'";

		if($author && $author !== $str->trans('author')) $author_sql = " and e.contact_name like '%".urldecode($author)."%'";
		if($keyword && $keyword !== $str->trans('keyword')) $keyword_sql = " and (e.title like '%".urldecode($keyword)."%' or e.description like '%".urldecode($keyword)."%')";
		if($language_id) $language_id_sql = " and e.language_id = '".$language_id."' ";

		if($year|$month|$day) $year_sql = " and start_date like '%". $year . $month . $day ."%'";

		$db = new DB;
		$sql = "select e.event_id, e.title, e.start_date,".$description." l.name as lname, t.name as tname, o.name as oname from event e, location l, event_type t, event_topic o ";
		$sql .= "where e.location_id = l.location_id and e.event_type_id = t.event_type_id and e.event_topic_id = o.event_topic_id ";
		$sql .= "$location_sql $eventtopic_sql $eventtype_sql $author_sql $language_id_sql $keyword_sql $year_sql";
		$sql .= " order by event_id $sort_sql limit $pager,$limit";
		$result = $db->query($sql);

		$tblhtml = "<table>";
		while($row = array_pop($result))
		{
		  $tblhtml .= "<tr><td valign=\"top\">".$row['event_id']."</td>";
		  $tblhtml .= "<td valign=\"top\"><a href=\"".SF_CALENDAR_URL."/event_display_detail.php?event_id=".$row['event_id']."\"";
		  $tblhtml .= "target=\"_blank\">".$row['title']."</a><br>";
		  $tblhtml .= $row['start_date']."<br>";
		  if($summary !== "no") { $tblhtml .= $row['description']."<br>"; }
		  $tblhtml .= "<b>".$tr->trans('location')."</b>: ".$row['lname']." ";
		  $tblhtml .= "<b>".$tr->trans('event_type')."</b>: ".$row['tname']." ";
		  $tblhtml .= "<b>".$tr->trans('event_topic')."</b>: ".$row['oname']."<br><br></td>";
		  $tblhtml .= "<td valign=\"top\"><a href=\"calendar_edit.php?event_id=".$row['event_id']."\">".$tr->trans('edit')."</a></td>";
		  $tblhtml .= "<td valign=\"top\"><a href=\"calendar_delete.php?event_id=".$row['event_id']."\">".$tr->trans('delete')."</a></td></tr>";
		 }
		 $tblhtml .= "</table>";

		 $this->tkeys['tablehtml'] = $tblhtml;



		// here we create the searchform.

        $event_renderer = new EventRenderer();
        $date_renderer = new DateRenderer();
		$location_obj = new Location();
		$eventtopic_obj = new Eventtopic();
		$eventtype_obj = new Eventtype();
		$tpl = new FastTemplate(SF_TEMPLATE_PATH."/pages/");
		$tpl->clear_all();
		$tpl->define(array(search => 'calendar_search.tpl'));
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

		$search_vars["TPL_SEARCH_YEAR"] = "<Select name=\"year\">\n";
		$search_vars["TPL_SEARCH_YEAR"] .='<Option value="">'.$tr->trans('year').'</OPTION>';
		$search_vars["TPL_SEARCH_YEAR"] .= $event_renderer->create_dropdown($date_renderer->get_years(),$year);
		$search_vars["TPL_SEARCH_YEAR"] .= "</Select>";

		$search_vars["TPL_SEARCH_MONTH"] = "<Select name=\"month\">\n";
		$search_vars["TPL_SEARCH_MONTH"] .='<Option value="">'.$tr->trans('month').'</OPTION>';
		$search_vars["TPL_SEARCH_MONTH"] .= $event_renderer->create_dropdown($date_renderer->get_months(),$month);
		$search_vars["TPL_SEARCH_MONTH"] .= "</Select>";

		$search_vars["TPL_SEARCH_DAY"] = "<Select name=\"day\">\n";
		$search_vars["TPL_SEARCH_DAY"] .='<option value="">'.$tr->trans('day')."</OPTION>";
        $search_vars["TPL_SEARCH_DAY"] .= $event_renderer->create_dropdown($date_renderer->get_days_of_month(),$day);
        $search_vars["TPL_SEARCH_DAY"] .= "</Select>";

		$location_options[''] = $tr->trans('locations');
        $event_type_options[''] = $tr->trans('event_types');
        $event_topic_options[''] = $tr->trans('event_topics');
        $search_vars['TPL_SEARCH_LOCATION'] = $this->make_select_form('location', $location_options, $location);
        $search_vars['TPL_SEARCH_EVENTTOPIC'] = $this->make_select_form('eventtopic', $event_topic_options, $eventtopic);
        $search_vars['TPL_SEARCH_EVENTTYPE'] = $this->make_select_form('eventtype', $event_type_options, $eventtype);



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

	$search_vars['TPL_ACTION'] = '';

		if ($summary == "no")
    	    $search_vars["TPL_SEARCH_SUMMARIES"]='checked="checked"';
	    else
			$search_vars["TPL_SEARCH_SUMMARIES"]="";

        include_once(SF_CACHE_PATH.'/language_options.inc');
        $language_options[''] = ' '.$tr->trans('select_a_language');
        asort($language_options);
        $search_vars['TPL_LANGUAGE_FORM'] = $this->make_select_form('language_id', $language_options, $language_id);

		$search_vars["TPL_FORM_NAME"] = "calendar_admin_top";


		$tpl->assign($search_vars);
		$tpl->parse("MAIN", "search");
		$searchhtml = $tpl->fetch(MAIN);
		$this->tkeys['form_search_top'] = $searchhtml;

		$tpl = new FastTemplate(SF_TEMPLATE_PATH."/pages");
		$tpl->clear_all();
		$tpl->define(array("bottom" => "calendar_search.tpl"));
		$search_vars["TPL_FORM_NAME"] = "calendar_admin_bottom";
		$tpl->assign($search_vars);
		$tpl->parse("MAIN", "bottom");
		$searchhtml = $tpl->fetch(MAIN);
		$this->tkeys['form_search_bottom'] = $searchhtml;
	}

}

?>
