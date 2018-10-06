<?php

// Class for display_by_date
include_once(SF_SHARED_PATH . '/classes/category_class.inc');

class archive_week_redirect extends Page
{

    function archive_week_redirect()
    {
        return 1;
    }

    function execute()
    {
		$db_obj=new SFACTIVE\DB();
		$category_obj=new SFACTIVE\Category;
		$category_id=$_GET["category_id"];
		if (!$category_id) $category_id=$GLOBALS['config_defcategory'];
		$category_fields=$category_obj->get_category_fields($category_id);
		$category_name=$category_fields['shortname'];
		if ($category_id==$GLOBALS['config_defcategory']) $category_name="";
		$date = new Date();
		$date->set_time(0,0,$_GET["day"], $_GET["month"], $_GET["year"]);
		$date->find_start_of_week();
		$category=$category_fields['category_id'];
		if ($category_name) $category_path="$category_name/";
		$filename = $GLOBALS['archive_cache_path']."/".$category_path.$date->get_weekname().".php";
		$fffp = fopen($filename, "r");
		//echo "writing to $filename";
		$cached_html = fread ( $fffp , filesize ($filename) );
			fclose($fffp);  		
			$this->tkeys['cached_archive_week'] = $cached_html;
			$this->tkeys['local_cat_id'] = $category_id;
			$this->tkeys['cat_shortname']=$category_fields['shortname'];
			$this->tkeys['cat_name']=$category_fields['name'];
			$this->tkeys['feature_url']=SF_FEATURE_URL.'/'.$this->tkeys['cat_shortname'].'/';
			if ($category_id==$GLOBALS['config_defcategory'])
			{ 
				$this->tkeys['feature_url']="/";
			}
		// added to show up banners ! :((( -- blicero 
		$GLOBALS['category_id']=$category_id;
		$GLOBALS['page_title']=$GLOBALS['dict']['archive_week'];
		return 1;
    }
}
?>
