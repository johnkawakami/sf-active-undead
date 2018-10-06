<?php // vim:et:ai:ts=4:sw=4

// Class for category_display_list page

class feature_list extends Page
{

    function feature_list()
    {
        return 1;
    }

    function execute()
    {

        $tr = new Translate();
        // create category dictionary
        $tr->create_translate_table('category');
        $category_obj = new SFACTIVE\Category();

        $category_list = $category_obj->get_active_list();

        while ($nextcategory = array_pop($category_list))
        {
	    //if (strlen($nextcategory['shortname'])>0 && $nextcategory['category_id']!=$GLOBALS['config_defcategory']){
            	$tblhtml .= "
<div class=\"hed\"><strong>&#8226;<a href=\"".SF_FEATURE_URL."/";
            	$tblhtml .= $nextcategory['shortname'] . "/\">";
            	$tblhtml .= $nextcategory['name'] . "</a></strong></div>";
	    	$tblhtml .= "<div class=\"featurebody\">".$nextcategory['description']."</div><br>";
	    //}
        }

        $this->tkeys['table_middle'] = $tblhtml;
        return 1;

    }

}

?>
