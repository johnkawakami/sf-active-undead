<?php

// Class for archive_generate_weeks

// fixme - this doesn't do anything.

class archive_generate_weeks extends Page
{

    function archive_generate_weeks ()
    {
        $tblhtml = "";
        include_once(SF_SHARED_PATH . '/classes/category_class.inc');
        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');
        include_once(SF_SHARED_PATH . '/classes/date_class.inc');

        $local_archive_cache_path = SF_ARCHIVE_PATH . '/cache';

        $day = $_GET['day'];
        $year = $_GET['year'];
        $month = $_GET['month'];
        $numtogen = $_GET['numtogen'];
        $category_id = $_GET['category_id'];

        if ($category_id < 1)
        {
            $category_id = $GLOBALS['config_defcategory'];
        }

        return 1;
    }

    function execute()
    {
        // code goes in constructor for non-displaying pages
    }
}

?>
