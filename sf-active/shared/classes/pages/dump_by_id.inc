<?php

// Class for dump_by_id

class dump_by_id extends Page
{

    function dump_by_id()
    {
        if (!isset($_GET['feature_id'])) 
        {
            header ("Location: " . SF_ARCHIVE_URL . "/index.php");
            return 0;
        }

        return 1;
    }

    function execute()
    {

        $tblhtml = "";
        include_once(SF_SHARED_PATH . '/classes/category_class.inc');
        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');

        $db_obj = new DB;
        $feature_obj = new Feature;
        $category_obj = new Category;

        $body = $category_obj->render_single_feature_archive($_GET['feature_id']);

        $this->tkeys['local_rows'] = $body;
        return 1;
    }
}

?>
