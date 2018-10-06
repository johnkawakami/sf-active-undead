<?php

// Class for category_pushtoproduction page

class category_pushtoproduction extends Page
{

    function category_pushtoproduction()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess
        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');
        include_once(SF_SHARED_PATH . '/classes/category_class.inc');

        $category_obj = new \SFACTIVE\Category;
        $feature_obj  = new \SFACTIVE\Feature;

        $result_html = $category_obj->render_feature_list($_SESSION['category_id']);
        $category = $category_obj->get_category_fields($_SESSION['category_id']);
        $category_name = $category['shortname'];

        $fffp = fopen(SF_CACHE_PATH . "/center_column_" . $category_name . ".html", "w");
        fwrite($fffp, $result_html, strlen($result_html));
        fclose($fffp);

        header("Location: category_display_pushtoproduction_confirmation.php");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
