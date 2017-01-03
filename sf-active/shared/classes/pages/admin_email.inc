<?php
// Class for admin_email page
class admin_email extends Page
{
    function admin_email ()
    {
        // Class constructor, does nothing
        return 1;
    }
    function execute()
    {
        // mostly we are just assigning template variables here, publishing is done in publish class
        $tr = new Translate;
        $this->tkeys['local_success'] = '';
        $this->tkeys['local_sendto_address'] = $GLOBALS['mailing_list_news'];
        if ($_POST['action'] == "send")
        {
            if ($_POST['failsafe'])
            {
                $news_email = $GLOBALS['mailing_list_news'];
                $msubject = stripslashes($_POST['msubject']);
                $mbody = stripslashes($_POST['mbody']);
                mail($news_email,$msubject,$mbody,"From: $GLOBALS[site_nick] <$GLOBALS[public_email]>","-f$GLOBALS[public_email]");
                $this->tkeys['local_success'] = $tr->trans('your_mail_sent');
            } else
            {
                $this->tkeys['local_success'] = $tr->trans('your_mail_not_sent');
            }
        }
        $this->tkeys['local_msubject'] = $GLOBALS['mailing_list_subj'];
        $this->tkeys['local_nicedate'] = date("l, F j, Y");
        $this->tkeys['local_textarea'] = $GLOBALS['mailing_list_header'] . "\n\n\n" . $GLOBALS['mailing_list_footer'];
    }
} // end class
?>
