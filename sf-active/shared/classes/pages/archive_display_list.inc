<?php
include_once ("shared/global.cfg");
include_once (SF_SHARED_PATH."/classes/category_class.inc");
include_once (SF_SHARED_PATH."/classes/feature_class.inc");

// Class for category_display_edit page

class archive_display_list extends Page
{

    function archive_display_list()
    {
        return 1;
    }

    function execute()
    {

	$category_id=$_GET["category_id"];
	if ($category_id<1) {
		$category_id=$GLOBALS['config_defcategory'];
	}
	$status="a";
	$db_obj=new DB();
	$category_obj=new Category;
	$categoryfields=$category_obj->get_category_fields($category_id);
	$feature_obj=new Feature();
	$feature_list=$feature_obj->get_list($category_id, $status);
	$feature_list2=$feature_obj->get_list($category_id, 'c' );
	$feature_list = array_merge($feature_list, $feature_list2);
	$category=$category_id;
	$Title=$categoryfields['shortname'];
	$list_string="";
	while ($nextfeature=array_pop($feature_list)){
		$list_string.="<tr>\n";
		$list_string.="<td><a href=\"archive_by_id.php?id=".$nextfeature['feature_id']."&amp;category_id=".$category_id."\"><i>".$nextfeature['title1'].": </i>".$nextfeature['title2']."</a></td>\n";
		$list_string.="<td>".$nextfeature['display_date']."</td>";
		$list_string.="</tr>";
	}
	$this->tkeys['archive_list']=$list_string;
	$this->tkeys['cat_name']=$categoryfields['name'];
	$this->tkeys['feature_url']=SF_FEATURE_URL.'/'.$categoryfields['shortname'].'/';
	$this->tkeys['local_cat_id']=$category_id;
	$GLOBALS['page_title']=$GLOBALS['dict']['archive_id'];
}
}
?>
