<?php

// Class for authenticate page

class authenticate extends Page
{

    function authenticate()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

        include(SF_SHARED_PATH . '/classes/user_class.inc');

		//  let's truncate username and password to 30 chars     
 		$_POST['username1'] = substr($_POST['username1'],0,30);
 		$_POST['password'] = substr($_POST['password'],0,30);

		$fh = fopen('/tmp/sf-active.log','a+');
		fwrite($fh, serialize(array(date(DATE_RFC822),$_POST['username1'])) . "\n");
		fclose($fh);

        $user_obj = new User;

        if ($user_obj->authenticate($_POST['username1'], $_POST['password']))
        {
			ini_set('session.save_path', SF_SESSIONS_PATH);

            $user_id = $user_obj->get_user_id();
            $username = $_POST['username1'];

			//upate the last login -- mtoups 08/01/2004
			$db_obj= new DB;
			$query = 'UPDATE user SET lastlogin = NOW() where user_id = ' . $user_id;
			$error_num = $db_obj->execute_statement($query);

            session_start();
			session_regenerate_id(true);
            $_SESSION['username']  = $_POST['username1'];
            $_SESSION['user_id']   = $user_id;
            $_SESSION['is_editor'] = true;

			/* Here we check if the password is secure.
               If the password is insecure, we'll redirect the user to the page where he can change
               the password. This goes on until the password has been made secure.
            */
			$pass_security = $user_obj->check_pass_security($_POST['password']);
            if($pass_security === "ok")
			{ 
				$_SESSION['secure'] = "yes" ; 
			}else
			{
				$_SESSION['secure'] = "no" ;
			}

            session_write_close();

			if($_SESSION['secure'] !== "yes") { $goto = "/admin/user/user_display_edit.php?user_id1=".$_SESSION['user_id'] ; }
            elseif ($_POST['goto']) { $goto=$_POST['goto']; }
		    else { $goto='/admin/index.php'; }
            header("Location: $goto");
            exit;
        }else
        {
            $goto=$_POST['goto'];
            $goto=urlencode($goto);
            header("Location: /admin/authentication/authenticate_display_logon.php?logon_failed=true&goto=$goto");
            exit;
        }
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
