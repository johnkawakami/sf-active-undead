<?php

// Class for upload_display_add page

class upload_display_add extends Page {

    function upload_display_add() {
        // Class constructor, does nothing
        return 1;
    }
    
    function execute() 
    {

        $this->tkeys['local_update'] = '';
        $this->tkeys['local_uplimg'] = '';

        $this->translate = new Translate();
        $this->translate->create_translate_table($pageid);

        // Check if we are coming here from a form
        if (isset($_FILES['upload_file']['name'])) 
        {
            $copy_from = $_FILES['upload_file']['tmp_name'];

            $image_name = $_FILES['upload_file']['name'];
            $copy_to = SF_WEB_PATH . $_POST['upload_rel_path'] . $image_name;

			// if the file ends in anything but jpg jpeg gif or png, then fail
			if (!preg_match('/(jpg|jpeg|gif|png|mov|mp4|flv)$/i', $image_name))
			{
			    $crackmessage = "Filename didn't match a movie or image.  ";
			}
			// if the file ends in a script extension
			if (preg_match('/(asp|php|php3|pl|sh)$/i', $image_name))
			{
				// don't accept scripts
				$crackmessage = $_SERVER['REMOTE_ADDR']." tried to upload $path$image_name.";
			}
			// test the first line to see if it contains <?
			$fh = fopen( $copy_from, 'r' );
			$testline = fgets($fh);
			if (preg_match('/<\?/', $testline)) {
				$crackmessage = "First line of file matched a script.";
			}
			fclose($fh);
			if ($crackmessage) {
				$path = $_POST['upload_rel_path'];
			    $msg = $_SERVER['REMOTE_ADDR']." tried to upload $path$image_name.";
				$msg .= "  This email was sent to inform you of a potential crack on sf-active for la.indy. The following has not been uploaded. ";
				$msg .= " ".$crackmessage;
				$msg .= "Filename: ".$image_name;
				mail('collective@slaptech.net','attempt to upload non-image file',$msg);
				return 1;
			}

            $file_already_exists = false;

            if (file_exists($copy_to) && $_POST['overwrite'] == false) 
            {
                $file_already_exists = true;
                $this->update_error_status($this->translate->trans('image_exists_error'));
            } else 
            {
                if (!move_uploaded_file($copy_from, $copy_to)) 
                {
                    $this->update_error_status($this->translate->trans('image_cannot_move'));
                } else 
                {
                    $this->tkeys['local_uplimg'] = '<IMG SRC="' . $_POST['upload_rel_path'] . $image_name . '">';
                    $this->update_status($this->translate->trans('image_uploaded')." ".SF_ROOT_URL.$_POST['upload_rel_path'].$image_name);
                }
            }
	}

	$this->tkeys['dir_form'] = $this->make_select_form('upload_rel_path',$GLOBALS['admin_upload_dirs'],'');

	if (strlen($this->get_update_status()) > 0) $this->tkeys['local_update'] = $this->get_update_status();
	if (strlen($this->get_error_status()) > 0) $this->tkeys['local_update'] = $this->get_error_status();

        return 1;
    }
}

?>
