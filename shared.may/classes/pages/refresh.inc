<?php

// Class for refresh page

class refresh extends Page
{

    function refresh()
    {
        // Class constructor, does nothing
        return 1;
    }

    function execute()
    {

        include_once(SF_SHARED_PATH . '/classes/db_class.inc');
        include_once(SF_SHARED_PATH . '/classes/newswire_class.inc');
        include_once(SF_SHARED_PATH . '/classes/category_class.inc');
        // include_once(SF_SHARED_PATH . '/scripts/news_xml.inc');

        $output = "";
        $db_obj = new DB();
        $tr = new Translate();
        $category = new Category;
        $newswire = new Newswire;
        $template_obj = new FastTemplate(SF_TEMPLATE_PATH);

        $results = $tr->trans('nothing_was_done');

        if ($_GET['cat'])
        {
            if ($_GET['cat'] != "main")
            {
                $categorydata = $category->get_category_fields($_GET['cat']);
                if (is_array($categorydata) > 0) $Result .= $newswire->render_summary($categorydata);
            }

            $categorydata = $category->get_category_fields($GLOBALS['config_defcategory']);
            if (is_array($categorydata) > 0) $Result .= $newswire->render_summary($categorydata);
        }

        if ($Result)
        {
            $this->tkeys['local_results'] = $Result;
        } else
        {
            $this->tkeys['local_results'] = $results;
        }

        return 1;
    }

} // end class
?>
