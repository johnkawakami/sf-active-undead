<?php

// Class for article_display_list page

class article_display_list extends Page
{

    function article_display_list()
    {
        return 1;
    }

    function execute()
    {
        include_once(SF_SHARED_PATH . '/classes/calendar/event_renderer.inc');
        include_once(SF_SHARED_PATH . '/classes/calendar/date_renderer.inc');
        include(SF_SHARED_PATH . '/classes/calendar/default_object.inc');
	global $time_diff;
        $tr = new Translate();
        $tr->create_translate_table('newswire');
	// add listing of categories to each article entry
	$cat = new \SFACTIVE\Category();

        $event_renderer = new \SFACTIVE\Calendar\EventRenderer();
        $date_renderer = new DateRenderer();

        $summaries=$_GET['summaries'];
        $sort=$_GET['sort'];
        $author = $_GET['author'];
        $keyword = $_GET['keyword'];
        $page = intval($_GET['page']);
        if ($_GET['day'] and $_GET['day']<10) { $day = '0'.$_GET['day']; }
        else { $day = $_GET['day']; }
        if ($_GET['month'] and $_GET['month']<10) { $month = '0'.$_GET['month']; }
        else { $month = $_GET['month']; }
        $year = $_GET['year'];
        $comments = $_GET['comments'];    
        $medium = $_GET['medium'];
        $category = $_GET['category'];
        $display = $_GET['display'];
	$language_id = $_GET['language_id'] ;
		
	$limit_ids=false;
	if ($page<10 && strlen($year)==0 && strlen($comments)==0 && strlen($comments)==0){
		$limit_ids=true;        
        }

	$limit = $_GET['limit'];

        if (strlen($limit) < 1)
        {
            $limit=20;    
        }
        $display_status = array(
                                "l" =>  $tr->trans('status_local'),
                                "g" =>  $tr->trans('status_global'),
                                "t" =>  $tr->trans('status_display'),
                                "f" =>  $tr->trans('status_hidden')
                                );

// added for mail2admin4hiding ftr -- blicero
	include(SF_INCLUDE_PATH."/select_reason.inc");	

// edit all comment to an article ftr --blicero
	$parent_id_admin=$_GET['parent_id_admin'];

        $db_obj=new \SFACTIVE\DB();
        $tblhtml='';

// POST hidden input field to pass on in this form the GET variables :) -- blicero 03.10.2003
	
	$tblhtml.="<input type=\"hidden\" name=\"comments\" value=\"$comments\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"summaries\" value=\"$summaries\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"author\" value=\"$author\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"keyword\" value=\"$keyword\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"page\" value=\"$page\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"day\" value=\"$day\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"month\" value=\"$month\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"year\" value=\"$year\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"medium\" value=\"$medium\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"category\" value=\"$category\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"display\" value=\"$display\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"limit\" value=\"$limit\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"sort\" value=\"$sort\">\n";
	$tbmjtml.="<input type=\"hidden\" name=\"language_id\" value=\"$language_id\">\n";
	$tblhtml.="<input type=\"hidden\" name=\"parent_id_admin\" value=\"$parent_id_admin\">\n";

        $url_author=urlencode($author);
        $url_keyword=urlencode($keyword);

        if (!$limit) $limit = $GLOBALS['default_search_size'];
        $last=($page - 1);
        $next=($page + 1);

        if ($page<0) $page=0;

        $offset = ($page * $limit);
	
	$limiting_id="";
	if ($limit_ids){
		$query= "select max(id) as maxid from webcast";
		$result = $db_obj->query($query);
               	$limiting_id=0;
		$row = array_pop($result);
                $limiting_id=$row["maxid"];
		$limiting_id=$limiting_id-5000;	
	}

        if ($category)
        {
            $query = "select w.display as display,w.id as
id,date_format(w.created + interval         ".$time_diff." second,'%Y-%m-%d %h:%i %p') as date_entered,w.author
as author,w.heading as heading,w.mime_type as mime_type,w.parent_id as
parent_id,w.created as created, date_format(w.created, '%Y') as
created_year, date_format(w.created,'%m') as created_month, w.language_id as language_id, ";
    if (!$summaries) $query .= 'w.summary as summary,';
	$query .= " w.modified as modified,w.numcomment as numcomment,w.arttype as
arttype,w.article as article from webcast w,catlink l where";
        } else
        {
            $query = "select display,id,date_format(created + interval         ".$time_diff." second,'%Y-%m-%d %h:%i %p') as date_entered,author,heading,mime_type,parent_id,created,
date_format(created, '%Y') as created_year, date_format(created, '%m') as
created_month, language_id, ";
	 if (!$summaries) $query .= 'summary,';
	 $query .= " article,modified,numcomment,arttype from webcast where 1 ";
        }

	if (strlen($limiting_id)>0){
		if ($category) {
		$query .= " and w.id>$limiting_id ";
		}else{
		$query .= " and id>$limiting_id ";
		}
	}
        if (!$comments) $query .= " and parent_id=0";
        if (strlen($display) > 0)
        {
            $query .= " and display='" . $display . "'";
        } else
        {
            // $query .= " and display like '%'";
        }

        if ($category) $query .= " and w.id=l.id and l.catid=$category";
        if ($author) $query .= " and author='$author'";
        if ($medium == "all") $medium="";
	if ($language_id) $query .= " and language_id='$language_id'";
        if ($medium) $query .= " and mime_type like '%$medium%'";
        if ($keyword)
        {
            $query .= " and (summary like '%$keyword%'";
            $query .= " or article like '%$keyword%'";
            $query .= " or heading like '%$keyword%'";
            $query .= " or author like '%$keyword%')";
        }

        if ($year) $query .= " and created like '" . $year . $month . $day . "%'";
        if ($sort=="modified") $query .= " and numcomment>=1";

// added for edit all comment to an article ftr -- blicero
	if ($parent_id_admin) $query .= " and parent_id='$parent_id_admin'";

        if ($sort=="rating")
        {
            $query .= " ORDER BY rating DESC";
		}
        else if ($sort=="rank")
        {
            $query .= " ORDER BY numcomment DESC";
        } else if ($sort=="modified")
        {
            $query .= " ORDER BY modified DESC";
        } else if ($sort=="reverse")
        {
            $query .= " ORDER BY id ASC";
        } else
        {
            $query .= " ORDER BY id DESC";
        }


        $query .= " limit ".$offset."," . $limit;
		file_put_contents('/tmp/sf-active-adl-query',$query);
	// echo "!-- $query --";
	$result = $db_obj->query($query);
        
        if ($result) $numrows=count($result);
        
        //if ($numrows or $page) include("$shared_scripts_path/news-browser-nav.inc");
        if ($display == "f") $tblhtml .= "<h3>".$tr->trans('hidden_warning')."</h3>";
        if (($month and !$year) or ($day and !$month)) $tblhtml.= "<h3>".$tr->trans('date_warning')."</h3>";

        if ($numrows == 0)
        {
            $tblhtml.="<h2>".$tr->trans('no_results')."</h2>";
            if (preg_match("/ /",$keyword)) $tblhtml.= "<h4>".$tr->trans('search_suggestion')."</h4>";
        }

        if ($numrows > 0)
        {
            for ($cell_counter=0; $cell_counter<$numrows; $cell_counter++) {
                
                $row = array_pop($result);
                
                $edit_block="<br><a target=\"_blank\" href=\"article_edit.php?id=";
                $edit_block.=$row['id'];
                $edit_block.="\">";
                $edit_block.=$tr->trans('edit');
                $edit_block.="</a>&nbsp;&nbsp;";
		$edit_block.="<a href=\"".SF_ADMIN_URL."/feature/feature_display_edit.php?article_id=".$row['id']."\">".$tr->trans('promote_to_feature')."</a>&nbsp;&nbsp;";
                $edit_block.=$this->make_select_form("value_".$cell_counter, $display_status, $row['display']);
                $edit_block.="<input type=\"hidden\" name=\"id_".$cell_counter."\" value=\"".$row['id']."\">";
                $edit_block.="<input type=\"hidden\" name=\"old_display_".$cell_counter."\" value=\"".$row['display']."\">";
// added heading hidden input for mail2admin4hiding ftr -- blicero
                $edit_block.="<input type=\"hidden\" name=\"heading_".$cell_counter."\" value=\"".$row['heading']."\">";

                
                if ($row['parent_id'] != 0)
                {
                    if ($row['display']=="f")
                    {
                        $searchlink= SF_NEWS_URL."/hidden.php?id=".$row['parent_id']."#". $row['id'];
                    } else
                    {
                        $query="SELECT created FROM webcast WHERE id=".$row['parent_id'];
                        $idcheck = $db_obj->query($query);
                        $idrow=array_pop($idcheck);
                        $cachefile=$webroot_news_path."/".MakeCacheDirYearMonth($idrow['created_year'],$idrow['created_month']).$row['parent_id']."_comment.php";
                        $searchlink= SF_NEWS_URL."/".MakeCacheDir($idrow['created']).$row['parent_id']."_comment.php";
                        $searchlink.="#".$row['id'];
                        $idlink=$row['parent_id']."#".$row['id'];
                    }
                } else
                {
                    $cachefile=$webroot_news_path."/".MakeCacheDir($row['created']).$row['id'].".php";
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
                    $parent_id=$row['id']; $id="";
                } else
                {
                    $summary=substr(htmlentities(trim($row['article'])),0,300);
                }

                $mime_type = $row['mime_type'];
                if (preg_match("/text/",$mime_type)) $mime='text" src="'.SF_ROOT_URL.'/im/imc_article.gif" ';
                if (strlen($mime_type)<1) $mime='unknown" src="'.SF_ROOT_URL.'/im/imc_article.gif" ';
                if (preg_match("/image/",$mime_type)) $mime='image" src="'.SF_ROOT_URL.'/im/imc_photo.gif" ';
                if (preg_match("/audio/",$mime_type)) $mime='audio" src="'.SF_ROOT_URL.'/im/imc_audio.gif" ';
                if (preg_match("/video/",$mime_type)) $mime='video" src="'.SF_ROOT_URL.'/im/imc_video.gif" ';
                if (preg_match("/application/",$mime_type)) $mime='application" src="'.SF_ROOT_URL.'/im/imc_pdf.gif" ';
                if ($row['arttype']=="news-response") $mime='comment" src="'.SF_ROOT_URL.'/im/imc_comment.gif" ';
                
                //include("$shared_scripts_path/news-browser-display.inc");
                $tblhtml.="<div ";
                if (!is_int($cell_counter/2))

                $tblhtml.="class=\"bgsearchgrey\"";
                $tblhtml.="><b>".$row['id']."(".$row['parent_id'].")</b>";

                if ($row['display']=="f") $tblhtml.="<strike>";

                $tblhtml.="<a target=\"_blank\" name=\"".$cell_counter."\" href=\"".$searchlink."\"><img alt=\"".$mime."\" align=\"middle\" border=\"0\" /></a> 
                    <b><a target=\"_blank\" href=\"".$searchlink."\">".$heading."</a></b> 
                    <small>".$author."</small><br><i>".$row['date_entered']."</i>";

                if ($summary)
                {
                    $tblhtml.="<br /><font size=\"-2\">$summary</font> <a href=\"$searchlink\">(";
                    if (preg_match("/audio\/mpeg/",$row['mime_type'])) { $tblhtml.="MP3"; }
                    else if (preg_match("/mp4/",$row['mime_type'])) { $tblhtml.="MP4"; }
                    else if (preg_match("/real/",$row['mime_type'])) { $tblhtml.="realmedia"; }
                    else if (preg_match("/quicktime/",$row['mime_type'])) { $tblhtml.="quicktime"; }
                    else if (preg_match("/audio/",$row['mime_type'])) { $tblhtml.=$lang_audio; }
                    else if (preg_match("/image/",$row['mime_type'])) { $tblhtml.=$lang_image; }
                    else if (preg_match("/video/",$row['mime_type'])) { $tblhtml.=$lang_video; }
                    else if (preg_match("/pdf/",$row['mime_type'])) { $tblhtml.="PDF"; }
                    else { $tblhtml.=$lang_full_story; }
                    if ($row['numcomment']) $tblhtml.=" $lang_and ".$row['numcomment']." ";
                    if ($row['numcomment'] == 1) $tblhtml.=$lang_comment;
                    if ($row['numcomment'] > 1) $tblhtml.=$lang_comments;
                    $tblhtml.=")</a>";
                } 
                if ($row['display']=="f") $tblhtml.="</strike>";
                $tblhtml.=$edit_block;
// code for the mail2admin4hiding ftr :)))
		// creates a select 
		$select_reason=$this->make_select_form("reason_$cell_counter",$reason_options,$ar->article['reason']);
		$tblhtml.=$select_reason;
		$tblhtml.="<input type=\"text\" name=\"additional_reason_$cell_counter\" value=\"".$GLOBALS['dict']['additional_reason']."\">";
// code for having the list of categories at the end of the article -- blicero
        	$cat->get_article_catname_array($row['id']);
		if (is_array($cat->cat_name_match)) {
		$catlisting="<br>Cat:&nbsp;&nbsp;";
		while ($asd=(array_pop($cat->cat_name_match))) {
		$catlisting .= "&nbsp;&nbsp;".$asd;
			}
		}
		$tblhtml.= $catlisting;
// end code
                $tblhtml.="</div><br>";
                $i=$i+1;
        }
        $tblhtml.="<input type=\"hidden\" name=\"numsubmittedrows\" value=\"".$numrows."\">";
    }

        $this->tkeys['table_middle'] = $tblhtml;
   
//edited to add edit all comments to an article ftr
 
    $this->tkeys['nav']="<p>";
    if ($page) $this->tkeys['nav'].="<a href=\"$SCRIPT_NAME?author=$url_author&amp;category=$category&amp;comments=$comments&amp;day=$day&amp;display=$display&amp;hidden=$hidden&amp;keyword=$url_keyword&amp;limit=$limit&amp;medium=$medium&amp;month=$month&amp;page=$last&amp;sort=$sort&amp;year=$year&amp;parent_id_admin=$parent_id_admin&amp;language_id=$language_id\">&lt;&lt; ".$tr->trans('newer_stories')."</a>&nbsp;&nbsp;";

    $displaypage=$page+1;

    $this->tkeys['nav'].="$displaypage";

    if ($numrows == $limit){
         $this->tkeys['nav'].="&nbsp;&nbsp;<a href=\"$SCRIPT_NAME?author=$url_author&amp;category=$category&amp;comments=$comments&amp;day=$day&amp;display=$display&amp;hidden=$hidden&amp;keyword=$url_keyword&amp;limit=$limit&amp;medium=$medium&amp;month=$month&amp;page=$next&amp;sort=$sort&amp;year=$year&amp;parent_id_admin=$parent_id_admin&amp;language_id=$language_id\">".$tr->trans('more_stories')." &gt;&gt;</a>";
    }
    $this->tkeys['nav'].="</p>";

        $search_param_array = array();
     
        while(list($key, $value) = each($GLOBALS['dict']))
        {
            $keyid = "TPL_" . strtoupper($key);
            $search_param_arrays[$keyid] = $value;
        }
    reset($GLOBALS['dict']);
    
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
          "f" =>  $tr->trans('status_hidden')
        );
        $search_param_array["TPL_SEARCH_DISPLAY"]= $this->make_select_form("display", $display_options, $display);

        $GLOBALS['dropdown_min_year']= $GLOBALS['site_start_year'];
        $GLOBALS['dropdown_max_year']=date('Y')+1;

    $search_param_array["TPL_SEARCH_YEAR"] = "<Select name=\"year\">\n";
    $search_param_array["TPL_SEARCH_YEAR"] .='<Option value="">'.$tr->trans('year').'</OPTION>';
        $search_param_array["TPL_SEARCH_YEAR"] .= $event_renderer->create_dropdown($date_renderer->get_years(),$year);
        $search_param_array["TPL_SEARCH_YEAR"] .= "</Select>";

    $search_param_array["TPL_SEARCH_MONTH"] = "<Select name=\"month\">\n";
    $search_param_array["TPL_SEARCH_MONTH"] .='<Option value="">'.$tr->trans('month').'</OPTION>';
        $search_param_array["TPL_SEARCH_MONTH"] .= $event_renderer->create_dropdown($date_renderer->get_months(),$month);
        $search_param_array["TPL_SEARCH_MONTH"] .= "</Select>";

        $search_param_array["TPL_SEARCH_DAY"] = "<Select name=\"day\">\n";
	$search_param_array["TPL_SEARCH_DAY"] .='<option value="">'.$tr->trans('day')."</OPTION>";
        $search_param_array["TPL_SEARCH_DAY"] .= $event_renderer->create_dropdown($date_renderer->get_days_of_month(),$day);
        $search_param_array["TPL_SEARCH_DAY"] .= "</Select>";

        include_once(SF_CACHE_PATH.'/language_options.inc');
        $language_options[''] = " ".$tr->trans('select_a_language');
        asort($language_options);
        $search_param_array["TPL_SEARCH_LANGUAGE"] = $this->make_select_form('language_id', $language_options, $language_id);

    $medium_options = array(
                  "" =>  $tr->trans('medium'),
                  "text" =>  $tr->trans('text'),
          "video" =>  $tr->trans('video'),
                  "audio" =>  $tr->trans('audio'),
          "application" =>  $tr->trans('other')
        );
        $search_param_array["TPL_SEARCH_MEDIUM"]= $this->make_select_form("medium", $medium_options, $medium);

    $cat_obj = new \SFACTIVE\Category;
	$this->tr = $tr ;
	include_once(SF_CACHE_PATH."/cat_options.inc");

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
          "40" =>  "40",
                  "100" =>  "100",
          "200" =>  "200"
        );

        $search_param_array["TPL_SEARCH_LIMIT"]= $this->make_select_form("limit", $limit_options, $limit);

        $search_param_array["TPL_COMMENTS"]=$tr->trans('comments');
        $search_param_array["TPL_GO"]=$tr->trans('go');

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

