<?php

// Class for user_update page
include_once(SF_SHARED_PATH."/classes/user_class.inc");
class user_update extends Page {

    function user_update() {
        // In this kind of script, we can do everything in the constructor to avoid the messines
        $user_obj = new User();

		// we check if the password is safe
		if($user_obj->check_pass_security($_POST['password']) === "ok")
		{
			$user_fields = array(
                            user_id =>    $_POST['user_id1'],
                            username =>   $_POST['username1'],
                            password =>   $_POST['password'],
                            email =>      $_POST['email'],
                            phone =>      $_POST['phone'],
                            first_name => $_POST['first_name'],
                            last_name =>  $_POST['last_name']
                            );

        	$error_num = $user_obj->update($user_fields);
			if($_SESSION['secure'] !== "yes")
			{
				$_SESSION['secure'] = "yes" ;
				$new_loc = '../';
				header("Location: $new_loc");
				exit ;
			}else{
				header("Location: user_display_list.php");
				$new_loc = "user_display_edit.php?user_id1=".$_POST['user_id1'];
				header("Location: $new_loc");
				exit;
			}
		}else{
			$new_loc = "user_display_edit.php?user_id1=".$_POST['user_id1']."&insecure=yes";
			header("Location: $new_loc");
			exit;
		}
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
