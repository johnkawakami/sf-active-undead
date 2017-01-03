<?php

// Class for user_delete page

class user_delete extends Page {

    function user_delete() {
        // In this kind of script, we can do everything in the constructor to avoid the messines
 
        $user_obj = new User();
        $user_obj->delete($_GET[user_id1]);
        header("Location: user_display_list.php");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
