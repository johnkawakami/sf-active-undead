<?php

// Class for category_reorder page

class article_bulk_status_change extends Page
{

    function article_bulk_status_change()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

        //$article_obj = new Article;
        //$error_num = $article_obj->bulk_status_change($_GET);
    }

    function execute() {

// open a new translatoin issue
     $tr = new Translate();
        $tr->create_translate_table('newswire');
	      $display_status = array(
                                "l" =>  $tr->trans('status_local'),
                                "g" =>  $tr->trans('status_global'),
                                "t" =>  $tr->trans('status_display'),
                                "f" =>  $tr->trans('status_hidden')
                                );
	
// this POST-GET sequence is used to have the "Back to list" link actually bringin to the list of article one was working upon and not the default article list one can reach from the left-hand column of the admin interface -- blicero 03.10.2002

	if ($_POST[save]=="Save"){
	$author=$_POST['author'];
        $url_author=urlencode($author);
	$keyword=$_POST['keyword'];
        $url_keyword=urlencode($keyword);
	$page=$_POST['page'];
	$day=$_POST['day'];
	$month=$_POST['month'];
	$year=$_POST['year'];
	$comments=$_POST['comments'];	
	$medium=$_POST['medium'];
	$category=$_POST['category'];
	$display=$_POST['display'];
	$limit=$_POST['limit'];
	$sort=$_POST['sort'];
	$parent_id_admin=$_POST['parent_id_admin'];
	}else{
	$author=$_GET['author'];
        $url_author=urlencode($author);
	$keyword=$_GET['keyword'];
        $url_keyword=urlencode($keyword);
	$page=$_GET['page'];
	$day=$_GET['day'];
	$month=$_GET['month'];
	$year=$_GET['year'];
	$comments=$_GET['comments'];	
	$medium=$_GET['medium'];
	$category=$_GET['category'];
	$display=$_GET['display'];
	$limit=$_GET['limit'];
	$sort=$_GET['sort'];
	$parent_id_admin=$_GET['parent_id_admin'];
	}

	$numsubmittedrows=$_POST[numsubmittedrows];
        $resulthtml="";

	if ($numsubmittedrows!=""){

		$db_obj=new DB();
		
		$i=0;
		$j=0;
		$numchanged=0;
		$resulthtml.="<br><br><a href=\"/admin/article/index.php?author=$url_author&amp;category=$category&amp;comments=$comments&amp;day=$day&amp;hidden=$hidden&amp;keyword=$url_keyword&amp;limit=$limit&amp;medium=$medium&amp;month=$month&amp;page=$page&amp;sort=$sort&amp;year=$year&amp;parent_id_admin=$parent_id_admin\">Back to list</a><br><br>";
		while ($i<$numsubmittedrows+1){
			$next_id=$_POST["id_".$i];
			$next_value=$_POST["value_".$i];
			$old_value=$_POST["old_display_".$i];
	//added this 2 entries to have the variables i use in the mail -- blicero
			$headingtitle=$_POST["heading_".$i];
	                $hiding_reason=$_POST["reason_".$i];
			$hiding_optional_reason=$_POST["additional_reason_".$i];
			//echo $next_id."<br>";
			//echo $next_value."<br>";
			//echo $old_value."<br>";
			if ($next_value!=$old_value && (strlen($next_value)>0)){
				$cat_sel_query .= "id = '".$next_id."' or ";
				$article_obj = new Article;
				$article_obj->update_article_status($next_id, $next_value);
                        	$resulthtml.="<a target=\"_blank\" href=\"article_edit.php?id=$next_id\">";
				$resulthtml.=$next_id."</a>&nbsp;$old_value &gt; $next_value<br />";
				$numchanged++;
// mail2admin4hiding ftr -- blicero
			$email=$GLOBALS[notifymail];
			$from=$GLOBALS[admin_email];
			$subject=$GLOBALS['dict']['admin_hiding_message']. " -- ".$next_id;
			$message=$display_status[$next_value];
			$body="post ".$headingtitle." id nr ".$next_id."\n".$GLOBALS['dict']['edited_by']." ".$_SESSION['username']."\n".$message."\n".$GLOBALS['dict']['reason'].": ".$hiding_reason."\n ".$GLOBALS['dict']['additional_reason']." : ".$hiding_optional_reason;
// added control to have this configurable :) -- blicero 22 may 2003
			if ($GLOBALS['notify'] == "1" && eregi($next_value,$GLOBALS['notify_type'])) {
			  mail ($email, html_entity_decode($subject), html_entity_decode($body), "From:$from");
				}
			}
			$i++;

		}
		if ($numchanged){
			$category_obj=new Category();
			$newswire= new Newswire();
			$db_obj = new DB ;
			$query = "select category_id, shortname, summarylength, name, newswire from category ";
			$query .= "where category_id = '".$GLOBALS['config_defcategory']."'";
			$result = $db_obj->query($query);
			$query = "select distinct(c.category_id) as category_id, c.shortname, c.summarylength, c.name, c.newswire from category";
			$query .= " c, catlink l where c.category_id = l.catid and (";
			$query .= substr($cat_sel_query, 0, strlen($cat_sel_query) -3).")";

			$result2 = $db_obj->query($query);
			if(count($result2 > 0)) $result = array_merge($result, $result2);

			$art = new Article;
			$art->dump_webcast_latest();

			while($nextcat = array_pop($result))
            		{
				$newswire->render_summary($nextcat);
                                $newswire->make_newswire_rss($nextcat[category_id],$nextcat[shortname]);
			}
		}
		
    	}
	$this->tkeys['result_html'] = $resulthtml;
	}
}

?>
