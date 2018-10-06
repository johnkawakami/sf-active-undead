<?php // vim:et:ai:ts=4:sw=4

// Class for display_by_date

class archive_display_by_date extends Page
{

    function archive_display_by_date()
    {
        return 1;
    }

    function execute()
    {

        $tblhtml = "";
        include_once(SF_SHARED_PATH . '/classes/category_class.inc');
        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');

        $db_obj = new SFACTIVE\DB;
        $category_obj = new SFACTIVE\Category();
        $feature_obj = new SFACTIVE\Feature();
        $tr = new Translate();

        $page = $_GET['page'];
        $category_id = $_GET['category_id'];
        if (!$category_id) $category_id=$GLOBALS[config_defcategory];
		$names = $category_obj->get_category_fields($category_id);
		if($names) $this->category_fields_loaded=true;
		$this->tkeys['cat_shortname']=$names['shortname'];
		$this->tkeys['cat_name']=$names['name'];
		$this->tkeys['feature_url']=SF_FEATURE_URL.'/'.$this->tkeys['cat_shortname'].'/';
		if ($category_id==$GLOBALS['config_defcategory'])
		{
			$this->tkeys['feature_url']="/";
		}

        if (strlen($page) < 1)
        {
            $page = 1;
        }

        if (strlen($category_id) < 1)
        {
            $category_id = $GLOBALS['config_defcategory'];
        }
	if($this->category_fields_loaded !== true)
	{
	    $categoryfields = $category_obj->get_category_fields($category_id);
	}

        $category = $category_id;

        $template_obj= new FastTemplate(SF_TEMPLATE_PATH);
        $this->tkeys['local_html'] = $category_obj->render_feature_page($category_id, $page);

        if ($page > 1)
        {
            $this->tkeys['local_previous_page']  = "<a href=\"archive_display_by_date.php?category_id=";
            $this->tkeys['local_previous_page'] .= $category_id . "&amp;"."page=" . ($page-1) . "\">";
            $this->tkeys['local_previous_page'] .= $tr->trans('prev_archive_page') . "</A>";
        } else
        {
            $this->tkeys['local_previous_page'] = "";
        }
        $this->tkeys['page'] = $page;
        if (trim($this->tkeys['local_html']))
    {        
        $this->tkeys['local_next_page'] .= "<A href=\"archive_display_by_date.php";
    if (strlen($category_id) < 1){
        $this->tkeys['local_next_page'] .= "?page=";
    }else{
        $this->tkeys['local_next_page'] .= "?category_id=".$category_id . "&amp;"."page=";
    }
        $this->tkeys['local_next_page'] .= $page + 1 . "\">";
        $this->tkeys['local_next_page'] .= $tr->trans('next_archive_page') . "</a>";
    } else { $this->tkeys['local_next_page']=""; }
        $this->tkeys['local_cat_id'] = $category_id;
        $GLOBALS['page_title']=$GLOBALS['dict']['archive_date'];
        return 1;
    }
}

?>
