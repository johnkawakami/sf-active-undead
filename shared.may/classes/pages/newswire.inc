<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

// Class for article_display_list page

class newswire extends Page
{

    function newswire()
    {
        return 1;
    }

    function execute()
    {
        require_once(SF_SHARED_PATH . '/classes/category_class.inc');
        require_once(SF_SHARED_PATH . '/classes/calendar/event_renderer.inc');
        require_once(SF_SHARED_PATH . '/classes/calendar/date_renderer.inc');

        include(SF_SHARED_PATH . '/classes/calendar/default_object.inc');
	// added by blicero to fix time_diff errors
	global $time_diff;
        $tr = new Translate();
        $event_renderer = new \SFACTIVE\Calendar\EventRenderer();
        $date_renderer = new \DateRenderer();
        $db_obj=new \SFACTIVE\DB();

        $GLOBALS['page_title']=$tr->trans('newswire');
        $author=$_GET['author'];
	$keyword=$_GET['keyword'];
	$page=$_GET['page'];
        $day=$_GET['day'];
	$month=$_GET['month'];
	$year=$_GET['year'];
	$comments=$_GET['comments'];	
        $medium=$_GET['medium'];
        $category=$_GET['category'];
        $display=$_GET['display'];
        $sort=$_GET['sort']; 
	$summaries=$_GET['summaries'];
        $limit=$_GET['limit'];
	$language_id = $_GET['language_id'];
	$no_limited_ids=$GLOBALS['no_limited_ids'];
        $tblhtml='';
        $url_author=urlencode($author);
        $url_keyword=urlencode($keyword);
        if (!$limit) $limit = $GLOBALS['default_search_size'];
        $last=($page - 1);
        $next=($page + 1);
        if ($page<0) $page=0;
	$offset = ($page * $limit);
	if (!$no_limited_ids) {
		if ($page<2 && strlen($year)==0 && strlen($comments)==0 && strlen($comments)==0 && strlen($category)==0) {
			$limit_ids=true;        
	        }
	}
	if ($limit_ids) {
		$query= "select max(id) as maxid from webcast";
		$result = $db_obj->query($query);
               	$limiting_id=0;
		$row = array_pop($result);
                $limiting_id=$row["maxid"];
		$limiting_id=$limiting_id-5000;	
	}
	/**
	 * to speed up reads and prevent various locks, there are three read-only
	 * tables used to generate lists of search results.  yes, ugly, but search
	 * engines crawl the search results pages.  the tables are webcast_parents_l
	 * webcast_parents_t and webcast_parents. These are updated periodically by
	 * a cron job.  To take advantage of thes tables, the searching query must
	 * be modified to use the correct table.
	 */
	if ( !$comments && $display=='t' ) {
		$webcast = 'webcast_parents_t';
		$parents = '';
		$display_part = '';

	} else if ( !$comments && $display=='l' ) {
		$webcast = 'webcast_parents_l';
		$parents = '';
		$display_part = '';

	} else if ( !$comments ) {
		$webcast = 'webcast_parents';
		$parents = " and parent_id=0 ";
		switch($display) {
			case 'g':
				$display_part = " and display='g' ";
			break;
			case 'gl':
			case 'lg':
				$display_part = " and (display='l' or display='g')";
			break;
		}
	
	} else {
		$webcast = 'webcast';
		$display_part = " and display='$display' ";
		switch($display) {
			case 'gl':
			case 'lg':
				$display_part = " and (display='l' or display='g')";
			break;
			case 'lgt':
			case 'gtl':
			case 'tlg':
			case 'tgl':
			case 'glt':
			case 'ltg':
				$display_part = " and (display='l' or display='g' or display='t')";
			break;
		}
	}
// added to fix time_diff error -- blicero
	if ($category)
        {
			$query = "select w.display as display,w.id as id,date_format((w.created  + interval         ".$time_diff." second), '%Y-%m-%d  %l:%i %p') as date_entered,w.author as author,w.heading as heading,w.mime_type as mime_type,w.parent_id as parent_id,w.created as created, date_format(w.created, '%Y') as created_year, date_format(w.created, '%m') as created_month, w.language_id as language_id, ";
            if (!$summaries) $query .= 'w.summary as summary, w.linked_file as linked_file, ';
            $query .= "w.modified as modified,w.numcomment as numcomment,w.arttype as arttype from $webcast w,catlink l where 1";
        } else {
			$query = "select display,id,date_format((created  + interval         ".$time_diff." second), '%Y-%m-%d  %l:%i %p') as date_entered,author,heading,mime_type,parent_id,created, date_format(created, '%Y') as created_year, date_format(created, '%m') as created_month, language_id, ";
            if (!$summaries) $query .= 'summary, linked_file, ';
            $query .= "modified,numcomment,arttype from $webcast where 1 ";
	}
	if ((strlen($limiting_id)>0) && ($webcast=='webcast')){
		$query .= " and id>$limiting_id ";
	}
	
	$query .= " $parents ";

	$query .= " $display_part ";

	if ($category) $query .= " and w.id=l.id and l.catid=$category";
	if ($author) $query .= " and match(author) against('$author')";
	if ($medium == "all") $medium="";
	if ($language_id) $query .= " and language_id = '".$language_id."'";
	// do something to turn this into mime_type='image/jpeg' or mime_type='image/png' etc. -johnk fixme
	if ($medium) $query .= " and mime_type like '$medium%'";
	if ($keyword) {
	    $query .= " and (match(summary,article,heading,author) against('$keyword'))";
	}
	if ($year) $query .= " and created like '" . $year;
        if ($year and $month and $month<10) $query .= '0';
        if ($year) $query .= $month;
        if ($year and $month and $day and $day<10) $query .= '0';
        if ($year) $query .= $day . "%'";
	if ($sort=="modified") $query .= " and numcomment>=1";
	if ($sort=="rating") {
	    $query .= " ORDER BY rating DESC";
	} else if ($sort=="rank") {
	    $query .= " ORDER BY numcomment DESC";
	} else if ($sort=="modified") {
	    $query .= " ORDER BY modified DESC";
	} else if ($sort=="reverse") {
	    $query .= " ORDER BY id";
	} else {
	    $query .= " ORDER BY id DESC";
	}
	$query .= " limit ".$offset."," . $limit;
	
	if (strpos( $query, 'against' ) > 0) {
		$result = array( array( "parent_id"=>0, "heading"=>"Search feature is off for now, due to server load problems.", "id"=>261355, "date_entered"=>"2013/08/23 12:21:00", "created"=>"20130823000000", "created_year"=>"2013", "created_month"=>"08", "summary"=>"Read this for details....") );
	} else {
			file_put_contents('/tmp/sf-active-newswire-query',$query);
			$result = $db_obj->query($query);
	}

	if ($result) $numrows=count($result);

        if ($display == "f") $tblhtml .= "<h3>".$tr->trans('hidden_warning')."</h3>";
        if (($month and !$year) or ($day and !$month)) $tblhtml .=  "<h3>".$tr->trans('date_warning')."</h3>";
        if ($numrows == 0)
        {
            $tblhtml.="<h2>".$tr->trans('no_results');
            if (preg_match("/ /",$keyword)) $tblhtml .="<h4>".$tr->trans('search_suggestion')."</h4>";
            $tblhtml.="</h2>";
        }
		
	if ($numrows > 0) 
        {
	    for ($cell_counter=0; $cell_counter<$numrows; $cell_counter++)
            {
	        $row = array_pop($result);
		$image_html = 0 ;
	        if ($row['parent_id'] != 0 && $row['parent_id'] != '')
                {
		    if ($row['display']=="f")
                    {
		        $searchlink= SF_NEWS_URL."/hidden.php?id=".$row['parent_id']."#".$row['id'];
		    } else {
		        $query="SELECT created FROM webcast WHERE id=".$row['parent_id'];
			$idcheck = $db_obj->query($query);
			$idrow=array_pop($idcheck);
			$cachefile= SF_NEWS_URL."/".MakeCacheDirYearMonth($idrow['created_year'],$idrow['created_month']).$row['parent_id']."_comment.php";
			$searchlink="/news/".MakeCacheDir($idrow['created']).$row['parent_id']."_comment.php";
			$searchlink.="#".$row['id'];
			$idlink=$row['parent_id']."#".$row['id'];
		    }
		} else {
		    $cachefile= SF_NEWS_URL."/".MakeCacheDir($row['created']).$row['id'].".php";
		    $searchlink= SF_NEWS_URL."/".MakeCacheDir($row['created']).$row['id'].".php";
		    $idlink=$row['id'];
		}
		$parent_id=$row['parent_id'];
		$search=array("'\''","'<'","'>'");
		$replace=array("&#039;","&lt;","&gt;");
		$heading=preg_replace($search,$replace,$row['heading']);
		$author=preg_replace($search,$replace,$row['author']);
		$summary=preg_replace($search,$replace,$row['summary']);
		if ($parent_id=="0")
                {
                    $parent_id=$row['id'];
                    $id="";
                }
		$mime_type = $row['mime_type'];
		if (preg_match("/text/",$mime_type)) $mime='text" src="'.SF_ROOT_URL.'/im/imc_article.gif" ';
		if (strlen($mime_type)<1) $mime='unknown" src="'.SF_ROOT_URL.'/im/imc_article.gif" ';
		if (preg_match("/image/",$mime_type))
                {
                    $mime='image" src="'.SF_ROOT_URL.'/im/imc_photo.gif" ';
                    $linked_file = basename($row['linked_file']);

                    $img_url = SF_UPLOAD_URL .'/'.$row['created_year'].'/'.$row['created_month'].'/'.$linked_file;
                    $img_path = SF_UPLOAD_PATH.'/'.$row['created_year'].'/'.$row['created_month'].'/'.$linked_file;
                    $ext = substr($linked_file, strrpos($linked_file, '.'), strlen($linked_file));
                    $thumb_url = $img_url . 'thumb' . $ext ;
                    $thumb_path = $img_path .'thumb' . $ext ;
                    if(file_exists($thumb_path))
                    {
                         //$image_info = getimagesize($thumb_url
                         $image_html = '<img src="'.$thumb_url.'" alt="'.$row['heading'].'" border=\"0\" />';
                    }else{
                         $image_html = '';
                    }
                }

		if (preg_match("/audio/",$mime_type)) $mime='audio" src="'.SF_ROOT_URL.'/im/imc_audio.gif" ';
		if (preg_match("/video/",$mime_type)) $mime='video" src="'.SF_ROOT_URL.'/im/imc_video.gif" ';
		if (preg_match("/application/",$mime_type)) $mime='application" src="'.SF_ROOT_URL.'/im/imc_pdf.gif" ';
		if ($row['arttype']=="news-response") $mime='comment" src="'.SF_ROOT_URL.'/im/imc_comment.gif" ';
		$tblhtml.="<p><a name=\"".$cell_counter."\" href=\"".$searchlink."\">
		<img alt=\"".$mime."\" align=\"middle\" border=\"0\" /></a> 
		<b><a href=\"".$searchlink."\">".$heading."</a></b> 
		<small>".$author." 
		<i>".$row['date_entered']."</i></small>";
                if ($summary)
                {
		    if (preg_match("/image/",$mime_type)) $tblhtml .= "<br /><a href=\"$searchlink\">$image_html</a>";
                    $tblhtml .= "<br />$summary <a href=\"$searchlink\">(";
                    $tblhtml .= $row['mime_type'];
                    if ($row['numcomment']) $tblhtml .= " + ". $row['numcomment']. " ";
                    if ($row['numcomment'] == 1) $tblhtml .= $tr->trans('single_comment');
                    if ($row['numcomment'] > 1) $tblhtml .= $tr->trans('plural_comment');
                    $tblhtml .= ")</a>";
                }elseif(!$summary && $image_html){
		    $tblhtml .= "<br /><a href=\"$searchlink\">$image_html</a>";
		}
                if ($summaries and $row['numcomment'])
                {
                    $tblhtml .= '<small> ('.$row['numcomment'].' ';
                    if ($row['numcomment']==1) $tblhtml .= $tr->trans('single_comment');
                    if ($row['numcomment']>1) $tblhtml .= $tr->trans('plural_comment');
                    $tblhtml .= ')</small>';
                }                
                $tblhtml .= "</p>";
	    }
	}
// i made this center so it's easier to see & google style
	$this->tkeys['nav']='<p align="center">';
	if ($page) $this->tkeys['nav'].="<a href=\"$SCRIPT_NAME?author=$url_author&amp;category=$category&amp;comments=$comments&amp;day=$day&amp;display=$display&amp;hidden=$hidden&amp;keyword=$url_keyword&amp;limit=$limit&amp;medium=$medium&amp;month=$month&amp;page=$last&amp;sort=$sort&amp;summaries=$summaries&amp;year=$year&amp;language_id=$language_id\">&lt;&lt; ".$tr->trans('newer_stories')."</a>&nbsp;&nbsp;";
        $displaypage=$page+1;
        $this->tkeys['nav'].="$displaypage";
        if ($numrows == $limit)
        {
	    $this->tkeys['nav'].="&nbsp;&nbsp;<a href=\"$SCRIPT_NAME?author=$url_author&amp;category=$category&amp;comments=$comments&amp;day=$day&amp;display=$display&amp;hidden=$hidden&amp;keyword=$url_keyword&amp;limit=$limit&amp;medium=$medium&amp;month=$month&amp;page=$next&amp;sort=$sort&amp;summaries=$summaries&amp;year=$year&amp;language_id=$language_id\">".$tr->trans('more_stories')." &gt;&gt;</a>";
        }
        $this->tkeys['nav'].="</p>";
        $this->tkeys['table_middle'] = $tblhtml;
        $search_param_array = array();
        while(list($key, $value) = each($GLOBALS['dict']))
        {
            $keyid = "TPL_" . strtoupper($key);
            $search_param_arrays[$keyid] = $value;
        }
        $search_param_array["TPL_COMMENTS"]=$tr->trans('comments');
        if ($_GET['author']) 
        {
            $search_param_array["TPL_SEARCH_AUTHOR"]=$_GET['author'];
        } else {
            $search_param_array["TPL_SEARCH_AUTHOR"]=$tr->trans('author');
        }
        if ($_GET['keyword'])
        {
            $search_param_array["TPL_SEARCH_KEYWORD"]=$_GET['keyword'];
	} else {
            $search_param_array["TPL_SEARCH_KEYWORD"]=$tr->trans('keyword');
        }
        $search_param_array["TPL_KEYWORD"]=$tr->trans('keyword');
        $search_param_array["TPL_AUTHOR"]=$tr->trans('author');
        $search_param_array["TPL_GO"]=$tr->trans('go');
        $search_param_array["TPL_NO_SUMMARIES"]=$tr->trans('no_summaries');

	if ($comments)
		$search_param_array["TPL_SEARCH_COMMENTS"]='checked="checked"';
	else
		$search_param_array["TPL_SEARCH_COMMENTS"]="";

	if ($summaries)
		$search_param_array["TPL_SEARCH_SUMMARIES"]='checked="checked"';
	else
		$search_param_array["TPL_SEARCH_SUMMARIES"]="";
	
	$display_options = array(
                  "" =>  $tr->trans('all'),
                  "l" =>  $tr->trans('status_local'),
		  "g" =>  $tr->trans('status_global'),
                  "t" =>  $tr->trans('status_display'),
		  "f" =>  $tr->trans('status_hidden'),
                  "lg" => $tr->trans('status_both')
        );
        $search_param_array["TPL_SEARCH_DISPLAY"]= $this->make_select_form("display", $display_options, $display);

// may be should be sfactive config site_start_year (this is done now :-) -- bart
        $GLOBALS['dropdown_min_year']=$GLOBALS['site_start_year'];
        $GLOBALS['dropdown_max_year']=date('Y')+1;

	$search_param_array["TPL_SEARCH_YEAR"] = "<Select name=\"year\">\n";
	$search_param_array["TPL_SEARCH_YEAR"] .="<Option value=''>".$tr->trans('year')."</OPTION>";
        $search_param_array["TPL_SEARCH_YEAR"] .= $event_renderer->create_dropdown($date_renderer->get_years(),$year);
        $search_param_array["TPL_SEARCH_YEAR"] .= "</Select>";

	$search_param_array["TPL_SEARCH_MONTH"] = "<Select name=\"month\">\n";
	$search_param_array["TPL_SEARCH_MONTH"] .="<Option value=''>".$tr->trans('month')."</OPTION>";
        $search_param_array["TPL_SEARCH_MONTH"] .= $event_renderer->create_dropdown($date_renderer->get_months(),$month);
        $search_param_array["TPL_SEARCH_MONTH"] .= "</Select>";

	$search_param_array["TPL_SEARCH_DAY"] = "<Select name=\"day\">\n";
	$search_param_array["TPL_SEARCH_DAY"] .="<Option value=''>".$tr->trans('day')."</OPTION>";
        $search_param_array["TPL_SEARCH_DAY"] .= $event_renderer->create_dropdown($date_renderer->get_days_of_month(),$day);
        $search_param_array["TPL_SEARCH_DAY"] .= "</Select>";

        include_once(SF_CACHE_PATH.'/language_options.inc');
        $language_options[''] = " ".$tr->trans('select_a_language');
        asort($language_options);
        $search_param_array["TPL_SEARCH_LANGUAGE"] = $this->make_select_form('language_id', $language_options, $language_id);


	$medium_options = array(
                  "" =>  $tr->trans('medium'),
                  "text" =>  $tr->trans('text'),
		  "image" =>  $tr->trans('image'),
                  "audio" =>  $tr->trans('audio'),
		  "video" =>  $tr->trans('video'),
		  "application" =>  $tr->trans('other')
        );
        $search_param_array["TPL_SEARCH_MEDIUM"]= $this->make_select_form("medium", $medium_options, $medium);

        //$cat_obj = new Category;
        //$category_options = $cat_obj->get_newswire_assoc_list();
	$this->tr = new Translate ;
	include_once(SF_CACHE_PATH.'/cat_options.inc');

        $search_param_array["TPL_SEARCH_CATEGORY"]= $this->make_select_form("category", $cat_options, $category);


	$sort_options = array(
                  "" =>  $tr->trans('sortby'),
                  "selected" =>  $tr->trans('newest'),
		  "reverse" =>  $tr->trans('reverse'),
                  "modified" =>  $tr->trans('modified'),
		  "rank" =>  $tr->trans('rank'),
		  "rating" =>  $tr->trans('rating')
        );

$search_param_array["TPL_SEARCH_SORT"]=$this->make_select_form("sort",$sort_options, $sort);

	$limit_options = array(
                  "10" =>  "10",
                  "20" =>  "20",
		  "30" =>  "30",
                  "40" =>  "40",
		  "50" =>  "50",
		  "100" => "100",
		  "200" => "200"
        );
	if($GLOBALS['default_search_size'] !==(10|20|30|40|50|100|200))
		{
			$x = $GLOBALS['default_search_size'];
			$limit_options["$x"] = $x;
	                asort($limit_options);
		}

        $search_param_array["TPL_SEARCH_LIMIT"]= $this->make_select_form("limit", $limit_options, $limit);

        $search_param_array["TPL_FORM_NAME"]='top_form';

	$search_template_class = new FastTemplate(SF_TEMPLATE_PATH . "/pages");
	$search_template_class->clear_all();
        $search_template_class->define(array('page' => "newswire_search.tpl"));
	$search_template_class->assign($search_param_array);
        $search_template_class->parse('CONTENT',"page");
        $top_search_form = $search_template_class->fetch("CONTENT");
        $this->tkeys['top_search_form'] = $top_search_form;

        $search_param_array["TPL_FORM_NAME"]='bottom_form';
	$search_template_class->assign($search_param_array);
        $search_template_class->parse('CONTENT',"page");
        $bottom_search_form= $search_template_class->fetch("CONTENT");
        $this->tkeys['bottom_search_form'] = $bottom_search_form;
	
	return 1;

    }

}

