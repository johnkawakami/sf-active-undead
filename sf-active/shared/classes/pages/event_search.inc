<?php

// Class for event_search page

class event_search extends Page {

    function event_search() {
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


		$get = filter_input_array(INPUT_GET, array(
			'author' => FILTER_SANITIZE_STRING,
			'keyword' => FILTER_SANITIZE_STRING,
			'display' => FILTER_SANITIZE_STRING,
			'eventtopic' => FILTER_SANITIZE_STRING,
			'eventtype' => FILTER_SANITIZE_STRING,
			'summary' => FILTER_SANITIZE_STRING,
			'location' => FILTER_SANITIZE_STRING,
			'sort' => FILTER_SANITIZE_STRING,
			'language_id' => FILTER_VALIDATE_INT,
			'year' => FILTER_VALIDATE_INT,
			'month' => FILTER_VALIDATE_INT,
			'day' => FILTER_VALIDATE_INT,
			'page' => FILTER_VALIDATE_INT,
			'limit' => FILTER_VALIDATE_INT,
		));
		$author = $get['author'];
        $keyword = $get['keyword'];
        $page = $get['page'];
        if ($get['day'] and $get['day']<10) { $day = '0'.$get['day']; }
        else { $day = $get['day']; }
        if ($get['month'] and $get['month']<10) { $month = '0'.$get['month']; }
        else { $month = $get['month']; }
        $year = $get['year'];
        $display = $get['display'];
		$limit = $get['limit'];
		$summary = $get['summary'];
		$location = $get['location'];
		$eventtopic = $get['eventtopic'];
		$eventtype = $get['eventtype'];
		$sort = $get['sort'];
		$language_id = $get['language_id'];

		// the small navigation bar (<< 1 >>)
		if(!$limit) $limit = $GLOBALS['default_search_size'] ;
		if(!$page) { $page = 0; }
		$this_page = $page + 1;
		$prev = $page -1;
		$next = $page + 1;
		if($page > 0) { $prev_page = "<a href=\"?page=$prev&amp;limit=$limit&amp;summary=$summary&amp;sort=$sort&amp;location=$location&amp;eventtype=$eventtype&amp;eventtopic=$eventtopic&amp;year=$year&amp;month=$month&amp;day=$day&amp;author=$author&amp;keyword=$keyword&amp;language_id=$language_id";
			$prev_page .= "\">&lt;&lt;</a>" ; }
		else { $prev_page = ""; }
			$next_page = "<a href=\"?page=$next&amp;limit=$limit&amp;summary=$summary&amp;sort=$sort&amp;location=$location&amp;eventtype=$eventtype&amp;eventtopic=$eventtopic&amp;year=$year&amp;month=$month&amp;day=$day&amp;author=$author&amp;keyword=$keyword&amp;language_id=$language_id";
			$next_page .= "\">&gt;&gt;</a>";
		$pager = $page * $limit ;
		$this->tkeys['nav'] = "<div>$prev_page $this_page $next_page</div>";

		// sql modifiers
		if($summary =="no") { $description = '';}
		else{ $description = " e.description, e.linked_file, e.mime_type,";}

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
		$sql = "select e.event_id, e.title, e.start_date,".$description." l.name as lname, t.name as tname, o.name as oname, e.language_id as language_id from event e, location l, event_type t, event_topic o ";
		$sql .= "where e.location_id = l.location_id and e.event_type_id = t.event_type_id and e.event_topic_id = o.event_topic_id ";
		$sql .= "$location_sql $eventtopic_sql $eventtype_sql $language_id_sql $author_sql $keyword_sql $year_sql";
		$sql .= " order by event_id $sort_sql limit $pager,$limit";
		$result = $db->query($sql);

		$tblhtml = "<table>";
		while($row = array_pop($result))
		{
			// mangles the linked file path so it works on this server
			// previous inserts didn't put in a path relative to a known
			// root path, so it's an absolute path on zanon.
			$l = $row['linked_file'];
			$l = preg_replace('#^/usr/local/www-domains/#', '/www/', $l);
			$l = preg_replace('#^/usr/local/sf-active/#', '/www/', $l);
			$l = preg_replace('#^/mnt/ad3/usr/local/www-domains/#', '/www/', $l);
			$row['linked_file'] = $l;


		    $tblhtml .= "<tr><td valign=\"top\">".$row['event_id']."</td>";
		    $tblhtml .= "<td valign=\"top\"><a href=\"".SF_CALENDAR_URL."/event_display_detail.php?event_id=".$row['event_id']."\"";
		    $tblhtml .= "target=\"_blank\">".$row['title']."</a><br>";
		    $tblhtml .= $row['start_date']."<br>";
		    if($summary !== "no") 
		    {
			if($row['linked_file'] && file_exists($row['linked_file'].'thumb.jpg'))
			{
			    $info = getimagesize($row['linked_file'].'thumb.jpg');
			    $tblhtml .= '<img src="'.str_replace(SF_UPLOAD_PATH, SF_UPLOAD_URL, $row['linked_file'].'thumb.jpg').'"';
			    $tblhtml .= ' border="0" '.$info[3].' alt="'.$row['title'].'" /><br />';
			}
			$tblhtml .= $row['description']."<br>"; 
		    }
		    $tblhtml .= "<b>".$tr->trans('location')."</b>: ".$row['lname']." ";
		    $tblhtml .= "<b>".$tr->trans('event_type')."</b>: ".$row['tname']." ";
		    $tblhtml .= "<b>".$tr->trans('event_topic')."</b>: ".$row['oname']."<br><br></td></tr>";
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

		$search_vars["TPL_FORM_NAME"] = "calendar_admin_top";

		include_once(SF_CACHE_PATH.'/language_options.inc');
		$language_options[''] = ' '.$tr->trans('select_a_language');
		asort($language_options);
		$search_vars['TPL_LANGUAGE_FORM'] = $this->make_select_form('language_id', $language_options, $language_id);


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
	}

}

?>
