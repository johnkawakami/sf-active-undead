<?php

// Class for feature_display_history page

class feature_display_history extends Page
{

    function feature_display_history()
    {
        return 1;
    }

    function execute()
    {

        $tblhtml = "";
        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');

        $feature_obj = new \SFACTIVE\Feature;
        $feature_list = $feature_obj->get_history_list($_GET['feature_id']);

        global $user_dictionary;

        while ($nextfeature = array_pop($feature_list))
        { 

            $tblhtml .= "<TR";
            $tmp = $nextfeature['version_num'] / 2;
            if (!is_int($tmp))
            {
                $tblhtml .= " class=\"bgsearchgrey\"";
            }
            $tblhtml .= "><TD><A href=\"feature_display_edit.php?feature_version_id=";
            $tblhtml .= $nextfeature['feature_version_id'];
            $tblhtml .= "\">";
            $tblhtml .= $nextfeature['version_num'];
            $tblhtml .= "</A></TD><TD>";
            $tblhtml .= $nextfeature['title1'];
            $tblhtml .= "</td><td>";

            $modifier_name = $user_dictionary[$nextfeature['modifier_id']];
            if (strlen($modifier_name)<1) $modifier_name = "---";

            $tblhtml .= $modifier_name;
            $tblhtml .= "</td><td>" . $nextfeature['mod_date'] . "</TD></TR>";
        }

        $this->tkeys['local_table_rows'] = $tblhtml;
        return 1;
    }
}

?>
