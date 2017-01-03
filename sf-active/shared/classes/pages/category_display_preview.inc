<?php

// Class for category_display_preview page

class category_display_preview extends Page
{

    function category_display_preview()
    {
        return 1;
    }

    function execute()
    {

        $tblhtml = "";
        include_once(SF_SHARED_PATH . '/classes/category_class.inc');
        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');

        $category_obj = new Category;

        $this->tkeys['local_page_preview'] = $category_obj->render_feature_list($_SESSION['category_id']);
        return 1;
    }
}

?>