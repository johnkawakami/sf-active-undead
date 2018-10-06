<?php

// Class for mailinglist page

class mailinglist extends Page {

    function mailinglist() {
        // Class constructor, does nothing
        return 1;
    }
    function execute() {
        $tr = new Translate();
        $tr->create_translate_table('mailinglist');
        $this->tkeys['update'] = "";
        if ($_POST) {
            $mailmode = "";
            $email = trim($_POST['email']);
            // validate email address
            if (!eregi("^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$", $email))
            {
                $this->update_error_status($tr->trans('invalid_email'));
            } else {
                if (strlen($_POST['add_email']) > 0) {
                    $target = $GLOBALS['mailing_list_subscribe'] . '?email=' . urlencode($email);
		    if (fclose(fopen($target,'r'))) {
                        $this->update_status($tr->trans('subscribe_thanks'));
                    } else {
                        $this->update_error_status($tr->trans('subscribe_notsent'));
                    }
                }
                if (strlen($_POST['remove_email']) > 0) {
                    $target = $GLOBALS['mailing_list_url'] . '?email=' . urlencode($email) . '&amp;login-unsub=Unsubscribe';
		    if (fclose(fopen($target,'r'))) {
                        $this->update_status($tr->trans('unsubscribe_thanks'));
                    } else {
                        $this->update_error_status($tr->trans('unsubscribe_notsent'));
                    }
                }
                if (strlen($_POST['remind_email']) > 0) {
                    $target = $GLOBALS['mailing_list_url'] . '?email=' . urlencode($email) . '&amp;login-remind=Remind';
		    if (fclose(fopen($target,'r'))) {
                        $this->update_status($tr->trans('passwd_sent'));
                    } else {
                        $this->update_error_status($tr->trans('passwd_notsent'));
                    }
                }
            }
            $this->tkeys['update'] = $this->get_error_status() . $this->get_update_status();
        }
        return 1;
    }
}
?>
