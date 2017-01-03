<?php
/* vi:set ts=4 sw=4 ai: */
include_once ("shared/global.cfg");
include_once (SF_SHARED_PATH."/classes/category_class.inc");
include_once (SF_SHARED_PATH."/classes/feature_class.inc");

// Class for category_display_edit page

class archive_by_id extends Page
{

    function archive_by_id()
    {
        return 1;
    }

    function execute()
    {

	$category_id=$_GET["category_id"];
	if ($category_id<1) {
		$category_id=$GLOBALS['config_defcategory'];
	}
	$feature_id=$_GET["id"];
	if (! is_numeric($feature_id))
	{
		// code to block the ip.
		exit();
	}
	$this->tkeys['local_cat_id']=$category_id;

    $Category = new Category();
    $names = $Category->get_category_fields($category_id);
    $this->tkeys['cat_shortname']=$names['shortname'];
    $this->tkeys['cat_name']=$names['name'];
    $this->tkeys['feature_url']=SF_FEATURE_URL.'/'.$this->tkeys['cat_shortname'].'/';
    if ($category_id==$GLOBALS['config_defcategory'])
    { 
        $this->tkeys['feature_url']="/";
    }
	$filename = $GLOBALS['archive_cache_path']."/single/".$feature_id.".php";
    $fffp = fopen($filename, "r");
	$cached_html = fread ( $fffp , filesize ($filename) );
    fclose($fffp);  		

	$feature_obj = new Feature();
    $feature = $feature_obj->get_latest_feature($feature_id);
	//$temp_string=$feature_obj->render_feature($feature["feature_version_id"]);
	$this->tkeys['archive_cache'] = $cached_html;
    $GLOBALS['page_title']=$feature['title2'];
    $this->tkeys['page_title'] = htmlentities($feature['title2']);
    $this->tkeys['short_url'] = "/ar/{$feature_id}/{$category_id}";
    
    //$GLOBALS['page_title']=$GLOBALS['dict']['archive_id'];
	$GLOBALS['category_id']=$names['category_id'];
	$GLOBALS['page_description'] = $feature['summary'];
	// set some sharing globals - johnk
	return 1;
    }
}
?>
