<?php

// Class for user_add page
include_once(SF_SHARED_PATH."/classes/user_class.inc");
class user_add extends Page {

    function user_add() {
        // In this kind of script, we can do everything in the constructor to avoid the messines
 
        $user_obj = new User();
		if($user_obj->check_pass_security($_POST['password']) === "ok")
		{
	        $userfields=array(
                         'username' =>   $_POST['username1'],
                         'password' =>   $_POST['password'],
                         'email' =>      $_POST['email'],
                         'phone' =>      $_POST['phone'],
                         'first_name' => $_POST['first_name'],
                         'last_name' =>  $_POST['last_name']
                         );
		
        	$user_obj->add($userfields);
			header("Location: user_display_list.php");
			exit;
		}else{
			header("Location: user_display_edit.php?insecure=yes");
			exit;
		}
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
