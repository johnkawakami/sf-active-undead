<?php

// Class for feature_display_list page

class feature_display_list extends Page
{

    function feature_display_list()
    {
        return 1;
    }

    function execute()
    {

        include_once(SF_SHARED_PATH . "/classes/feature_class.inc");

        $tblhtml = "";
        $tr = new Translate();

        if (isset($_GET['category_id']) && isset($_GET['category_name']))
        {
            $_SESSION['category_id'] = $_GET['category_id'];
            $_SESSION['category_name'] = $_GET['category_name'];
        }

        $category_name2 = $_GET['category_name'];
        $_GET['category_name'] = $category_name2;

        $this->tkeys['local_subtitle'] = $tr->trans('features_edit');
        $this->tkeys['local_catid'] = $_SESSION['category_id'];
        $this->tkeys['local_category_name'] = $_SESSION['category_name'];

	$category_obj = new \SFACTIVE\Category;
        $category = $category_obj->get_category_fields($_SESSION['category_id']);
        $category_name = $category['shortname'];
	$category_cache = SF_CACHE_PATH . "/center_column_" . $category_name . ".html";
	if (file_exists($category_cache)) $category_cache_date = filemtime($category_cache);

        if (strlen($_GET['status']) == 0)
        {
            $_GET['status'] = "c";
        }

        $feature_obj = new \SFACTIVE\Feature();
        $feature_list = $feature_obj->get_list($_SESSION['category_id'], $_GET['status']);

        if ($_GET['status'] == "c" || strlen($_GET['status']) == 0)
        {
            $this->tkeys['local_current_link'] = $tr->trans('current');
        } else
        {
            $this->tkeys['local_current_link'] = "<A href=\"feature_display_list.php?status=c\">";
            $this->tkeys['local_current_link'] .= $tr->trans('current');
            $this->tkeys['local_current_link'] .= "</A></TD>";
        }

        if ($_GET['status'] == "a")
        {
            $this->tkeys['local_archived_link'] = $tr->trans('archived');
        } else
        {
            $this->tkeys['local_archived_link'] = "<A href=\"feature_display_list.php?status=a\">";
            $this->tkeys['local_archived_link'] .= $tr->trans('archived');
            $this->tkeys['local_archived_link'] .= "</A></TD>";
        }

        if ($_GET['status'] == "h")
        {
            $this->tkeys['local_hidden_link'] = $tr->trans('hidden');
        } else
        {
            $this->tkeys['local_hidden_link'] = "<A href=\"feature_display_list.php?status=h\">";
            $this->tkeys['local_hidden_link'] .= $tr->trans('hidden');
            $this->tkeys['local_hidden_link'] .= "</A></TD>";
        }

        if (!is_array($feature_list) || count($feature_list) < 1)
        {
// colspan to 9 (i added the test column :) 
            $tblhtml = "<TR><TD COLSPAN=\"9\"><CENTER>";
            $tblhtml .= $tr->trans('no_features_to_edit');
            $tblhtml .= "</CENTER></TD></TR>";

        } else
        {

            while ($nextfeature = array_pop($feature_list))
            {
                $tblhtml .= "<TR><TD width=20><CENTER><A href=\"feature_display_edit.php?feature_version_id=";
                $tblhtml .= $nextfeature['feature_version_id'];
                $tblhtml .= "\">";
                $tblhtml .= $nextfeature['feature_id'];
                $tblhtml .= "</A></CENTER></TD><TD width=200><A href=\"feature_display_edit.php?feature_version_id=";
                $tblhtml .= $nextfeature['feature_version_id'];
                $tblhtml .= "\">";
                $tblhtml .= $nextfeature['title1'];
                $tblhtml .= "</A></TD><TD>";
                $tblhtml .= $nextfeature['creation_date_month'];
                $tblhtml .= "/";
                $tblhtml .= $nextfeature['creation_date_day'];
                $tblhtml .= "/";
                $tblhtml .= $nextfeature['creation_date_year'];
                $tblhtml .= "</td><td";
                if (strtotime($nextfeature['modification_date']) > $category_cache_date) $tblhtml .= ' style="color: #cc0000; font-weight: bold"';
                $tblhtml .= ">";
                $tblhtml .= $nextfeature['modification_date_month'];
                $tblhtml .= "/";
                $tblhtml .= $nextfeature['modification_date_day'];
                $tblhtml .= "/";
                $tblhtml .= $nextfeature['modification_date_year'];
                $tblhtml .= "</TD><TD><INPUT size=4 name=\"feature_order";
                $tblhtml .= $nextfeature['feature_version_id'];
                $tblhtml .= "\" value=\"";
                $tblhtml .= $nextfeature['order_num'];
                $tblhtml .= "\" /></TD>";
// adde one column for test a ftr function :) -- blicero
                $tblhtml .= "<td>&nbsp;<a href=\"".SF_ARCHIVE_URL."/display_by_id.php?feature_id=";
		$tblhtml .= $nextfeature['feature_id'];
                $tblhtml .= "\" target=\"_blank\">". $tr->trans('test'). "</a></td>";

                if ($nextfeature['status'] != "a")
                {
                    $tblhtml .= "<TD>&nbsp;<A href=\"feature_change_status.php?status=a&feature_version_id=";
                    $tblhtml .= $nextfeature['feature_version_id'];
                    $tblhtml .= "\">" . $tr->trans('action_archive') . "</A></TD>";
                }

                if ($nextfeature['status'] != "c")
                {
                    $tblhtml .= "<TD>&nbsp;<A href=\"feature_change_status.php?status=c&feature_version_id=";
                    $tblhtml .= $nextfeature['feature_version_id'];
                    $tblhtml .= "\">" . $tr->trans('action_display') . "</A></TD>";
                }

                if ($nextfeature['status'] != "h")
                {
                    $tblhtml .= "<TD>&nbsp;<A href=\"feature_change_status.php?status=h&feature_version_id=";
                    $tblhtml .= $nextfeature['feature_version_id']."\"";
                    $tblhtml .= ">" . $tr->trans('action_hide') . "</A></TD></TR>";
                }
            }

        }

        $this->tkeys['local_table_rows'] = $tblhtml;
        return 1;

    }

}

?>
