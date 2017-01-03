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

		$db_obj=new DB();

		$date = new Date();
		$date->set_time(0,0,$_GET["day"], $_GET["month"], $_GET["year"]);
		$date->find_start_of_week();

		$category_obj=new Category;
		if (isset($_GET['category_id'])) {
			$category_id=filter_var($_GET["category_id"], FILTER_VALIDATE_INT);
		} else {
			$category_id=$GLOBALS['config_defcategory'];
		}

		$category_fields=$category_obj->get_category_fields($category_id);
		$category=$category_fields['category_id'];

		if ($category_id==$GLOBALS['config_defcategory']) {
			// why is this exceptional? probably pointless
			$filename = $GLOBALS['archive_cache_path']."/".$date->get_weekname().".php";
		} else {
			$filename = $GLOBALS['archive_cache_path']."/".$category_fields['shortname'].'/'.$date->get_weekname().".php";
		}

		if (file_exists($filename)) {
			$fffp = fopen($filename, "r");
			//echo "writing to $filename";
			$cached_html = fread ( $fffp , filesize ($filename) );
			fclose($fffp);  		
		} else {
			$cached_html = ''; // this is really an error condition.
		}
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
