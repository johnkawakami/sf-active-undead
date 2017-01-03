<?php

// Class for user_display_list page

class user_display_list extends Page {

    function user_display_list() {
        return 1;
    }

    function execute() {
 
        $user_obj = new User();
	if($_GET['order'])
	{
	    $user_list = $user_obj->get_ordered_list($_GET['order'], $_GET['style']);
	}else{
	    $user_list = $user_obj->get_list();
	}
 
        $tr = new Translate();
        $tr->create_translate_table('user_display_list');
	$tblhtml .= "<TR><TD>" . $tr->trans('username') . "</TD><TD></TD><TD>" . $tr->trans('lastlogin') . "</TD><TR>";

        while ($nextuser = array_pop($user_list)) {
            $tblhtml .= "<TR><TD><A href=\"user_display_edit.php?user_id1=";
            $tblhtml .= $nextuser[user_id] . "\">";
            $tblhtml .= $nextuser[username] . "</A></TD>";
            $tblhtml .= "<TD><A href=\"user_delete.php?user_id1=" . $nextuser[user_id];
            $tblhtml .= "\">" . $tr->trans('delete') . "</A></TD>";
	    $tblhtml .= "<TD>" . $nextuser[lastlogin] . "</TD>";
	    $tblhtml .= "</TR>\n";
        }

        $this->tkeys['table_middle'] = $tblhtml;

        return 1;
    }

}

?>
