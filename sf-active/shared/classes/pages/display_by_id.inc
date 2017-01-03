<?php

// Class for display_by_id

class display_by_id extends Page
{

    function display_by_id()
    {
        return 1;
    }

    function execute()
    {

        $tblhtml = "";
        include_once(SF_SHARED_PATH . '/classes/category_class.inc');
        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');

        if (!isset($_GET['feature_version_id']) and !isset($_GET['feature_id']))
        {
            header ("Location: ".SF_ARCHIVE_URL."/index.php");
        } else
        {
            $feature_version_id = $_GET['feature_version_id'];
            $feature_id = $_GET['feature_id'];
        }

        $db_obj = new DB;
        $feature_obj = new Feature;
        $category_obj = new Category;

        $feature_query = "  SELECT feature_version_id 
                            FROM feature
                            WHERE feature_id='$feature_id'
                            AND is_current_version='1'
                         ";

        $result_feature = $db_obj->query($feature_query);
        $result_feature = array_pop($result_feature);
        $feature_version_id = $result_feature['0'];

        $this->tkeys['local_html'] = $feature_obj->render_feature($feature_version_id);
        return 1;
    }
}

?>
