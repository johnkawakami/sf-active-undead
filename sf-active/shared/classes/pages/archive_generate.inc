<?php

// class to regenerate archives

include_once(SF_SHARED_PATH."/classes/category_class.inc");
include_once(SF_SHARED_PATH."/classes/feature_class.inc");
include_once(SF_SHARED_PATH."/classes/template_class.inc");
include_once(SF_SHARED_PATH . '/classes/date_class.inc');
class archive_generate extends Page
{

    function archive_generate()
    {
    	return 1;
    }

    function execute() {
	
	$category_obj= new Category();

	// set value for the archive-by-id regeneration and cycle trough feature id up until the regenration of idtogen feature	
	if (isset($_POST[submit_id])) {
	//$feature_obj= new Feature();
	//$template_obj= new FastTemplate(SF_TEMPLATE_PATH);
	$cycle=$_POST[idtogen];
	$feature_id=$_POST[feature_id];
	while ($cycle>0) {
		$msg.=$category_obj->render_single_feature_archive($feature_id);
	        $feature_id++;
        	$cycle=$cycle-1;
		}               	
	$this->tkeys[arcid_result]=$msg;
	$this->tkeys[value_featureid]=$feature_id;
	$this->tkeys[idtogen]=$_POST[idtogen];
	}else{
	$this->tkeys[arcid_result]='';
	$this->tkeys[value_featureid]='';
	$this->tkeys[idtogen]='';
	}

// set variables for archive week generation

	if (!$_POST[category_id]) { $category_id=$GLOBALS['config_defcategory'];} else { $category_id=$_POST[category_id];}
	if (!$_POST[day]) { $day=date(d); }else{ $day=$_POST[day];}
	if (!$_POST[month]) { $month=date(m);}else{ $month=$_POST[month];}
	if (!$_POST[year]) { $year=date(Y);}else{ $year=$_POST[year];}
	if (!$_POST[numtogen]) { $numtogen=5;}else{ $numtogen=$_POST[numtogen];}


	if (isset($_POST[submit])){

/// BEGIN CODE

	global $items_existed_for_week;
        $local_archive_cache_path = SF_ARCHIVE_PATH . '/cache';
	if (strlen($day)>0 && strlen($month)>0 && strlen($year)>0){
	$i=0;
	$week_date=new Date();
	$week_date->set_time(0,0,$day, $month, $year);
	$week_date->find_start_of_week();
	$this_week_date= new Date();
	$this_week_date->set_date_to_now();
	$this_week_date->find_start_of_week();	
	while ($i<$numtogen){
		$day=$week_date->get_day();
		$month=$week_date->get_month();
		$year=$week_date->get_year();
		$items_existed_for_week=false;
		
		if (!$categoryfields) $categoryfields=$category_obj->get_category_fields($category_id);
		$date= new Date();
		$date2=new Date();
		$date->set_time(0,0,$day,$month,$year);
		$date2->set_time(0,0,$day,$month,$year);
		$date2->move_n_weeks_forward(-1);
		$prev_week_year=$date2->get_year();
		$prev_week_month=$date2->get_month();
		$prev_week_day=$date2->get_day();
		$date2->move_n_weeks_forward(2);
		$next_week_year=$date2->get_year();
		$next_week_month=$date2->get_month();
		$next_week_day=$date2->get_day();
		$tr = new Translate ;
		$msg.= "<br>".$tr->trans('categoryid').": ".$category_id."<br />".$tr->trans('day').": ";
	 	$msg.=$month."/";
		$msg.=$day."/";
		$msg.=$year." <br />";
		$display_string=$category_obj->render_archives_for_week($category_id, $date);
		//if ( $items_existed_for_week ) { echo $display_string; } else { echo "diocane"; }
		if ($items_existed_for_week){ 
			if ($category_id!=$GLOBALS[config_defcategory]) {
				if (!$categoryfields) { 
					$categoryfields=$category_obj->get_category_fields($category_id); }
				$category_path="$categoryfields[shortname]/"; 
				if (!file_exists($local_archive_cache_path."/".$category_path)) {
					mkdir($local_archive_cache_path."/".$category_path,0775); } }
			$filename=$local_archive_cache_path."/".$category_path.$year."_".$month."_".$day.".php";
			$msg.= $tr->trans('writing').$filename."<br>";
			$fffp=fopen($filename, "w");
			fwrite($fffp, $display_string, strlen($display_string));
			fclose($fffp);
		}else{
			$msg.= $tr->trans('no_items_for_week').$year."-".$month."-".$day."<br>";
		}
		$week_date->move_n_weeks_forward(1);
		$i=$i+1;
	}
}else{
	$tr = new Translate ;
	$msg= $tr->trans('specify_dmy'); 
}


/// END CODE
	$this->tkeys[arcdate_result]=$msg;
	}else{
	$this->tkeys[arcdate_result]='';
	}
	$this->tkeys[value_day]=$day;
	$this->tkeys[value_month]=$month;
	$this->tkeys[value_year]=$year;
	$this->tkeys[value_catid]=$category_id;
	$this->tkeys[datetogen]=$numtogen;

	return 1;
    }

}

?>
