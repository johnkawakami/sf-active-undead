<?php

// Class for user_display_edit page

class user_display_edit extends Page {

    function user_display_edit() {
        return 1;
    }

    function execute() {
  
        $userid = '';
        $is_edit = false;
        $this->tkeys['local_form_action'] = "user_add.php";
        $this->tkeys['local_form_hidden'] = "";
        $this->tkeys['local_username'] =   "";
        $this->tkeys['local_password'] =   "";
        $this->tkeys['local_email'] =      "";
        $this->tkeys['local_phone'] =      "";
        $this->tkeys['local_first_name'] = "";
        $this->tkeys['local_last_name'] =  "";
	$this->tkeys['local_lastlogin'] = "";
	$this->tkeys['local_sec_warning'] = "";
	$this->tkeys['local_pass_desc'] = "";
	$this->tkeys['local_pass_desc2'] = "";

        if ($_POST['user_id1'] > 0) $userid = $_POST['user_id1'];
        if ($_GET['user_id1'] > 0) $userid = $_GET['user_id1'];

        if ($userid > 0) $is_edit = true;

        if ($is_edit) {
            $user_obj=new SFACTIVE\User();
            $user_detail=$user_obj->get_user_fields($userid);

            $this->tkeys['local_username'] =   $user_detail['username'];
            $this->tkeys['local_password'] =   $user_detail['password'];
            $this->tkeys['local_email'] =      $user_detail['email'];
            $this->tkeys['local_phone'] =      $user_detail['phone'];
            $this->tkeys['local_first_name'] = $user_detail['first_name'];
            $this->tkeys['local_last_name'] =  $user_detail['last_name'];
	    $this->tkeys['local_lastlogin'] =  $user_detail['lastlogin'];
            $this->tkeys['local_form_action'] = "user_update.php";
            $this->tkeys['local_form_hidden'] = "<INPUT TYPE=\"HIDDEN\" NAME=\"user_id1\" VALUE=\"" . $userid . "\">"; 
		}
		if(($_SESSION['user_id'] == $userid && $_SESSION['secure'] !== "yes") || $_GET['insecure'] == "yes")
		{
			$tr = new Translate ;
			$this->tkeys['local_sec_warning'] = "<big><b>".$tr->trans('insecure_pass_msg')."</b></big><br /><br />";
			$this->tkeys['local_pass_desc'] = $tr->trans('secure_pass_desc')."<br /><br />";
			$this->tkeys['local_pass_desc2'] = "<b>".$tr->trans('secure_pass_desc2')."</b><br /><br />";
		}
        
        return 1;
    }

}

?>
