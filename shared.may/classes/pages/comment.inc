<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

// Class for comment page
include_once(SF_SHARED_PATH."/classes/spam_class.inc");
include_once(SF_SHARED_PATH."/classes/craptcha_class.inc");
include_once(SF_SHARED_PATH."/classes/csrf_class.inc");
class comment extends Page
{

    function comment()
    {
        // Class constructor, does nothing
        return 1;
    }

    function execute()
    {

		$spam=new Spam();
		if ($_POST["publish"] != "") $spam_publish=1;
		if ($spam->Detect($spam_publish)==1)
		{
			// fake out the poster:
			echo "<br><br>* There were 0 file(s) detected.<br><br>";
			echo "* Your story is <a href='/'>http://la.indymedia.org/news/2010/04/01.php</a><br>";
			echo "* <b>Comment added!</b><br><br>";
			/*
				$tr = new Translate;
				echo "<h3>".$tr->trans('spam_message')."<h3>";
			*/
			echo "</body></html>";
			
			exit;
		}

        // mostly we are just assigning template variables here, publishing is done in publish class

        include_once(SF_SHARED_PATH . '/classes/pages/publish.inc');

        $tr = new Translate();
        $publish = new publish();
        $publish->generate_filebox_html();

        $max_num_uploads = array(0,1);
        $this->tkeys['select_filecount'] = $this->make_select_form("file_count", $max_num_uploads, $_POST['file_count']);

        if ($_GET['top_id'] > 0)  $top_id = $_GET['top_id'];
        if ($_POST['top_id'] > 0) $top_id = $_POST['top_id'];

        $text_options = array(
                             "t" => "text/plain",
                             "h" => "text/html"
                             );

        $this->tkeys['select_artmime'] = $this->make_select_form("artmime", $text_options, $_POST['artmime']);

        $this->tkeys['local_top_id'] = $top_id;
        $this->tkeys['file_boxes'] = $publish->filebox;
        $this->tkeys['pub_stepone'] = $tr->trans('comment_pub_one');
        $this->tkeys['pub_steptwo'] = $tr->trans('comment_pub_two');
        $this->tkeys['pub_stepthree'] = $tr->trans('comment_pub_three');
        $this->tkeys['pub_optional'] = $tr->trans('pub_optional');
        $this->tkeys['local_site_nick'] = $GLOBALS['site_nick'];
        $this->tkeys['pub_author'] = $tr->trans('author');
        $this->tkeys['pub_url'] = $tr->trans('link');
	include_once(SF_CACHE_PATH.'/language_options.inc');
	$language_options[''] = ' '.$tr->trans('select_a_language');
	asort($language_options);
	$this->tkeys['dropdown_language'] = $this->make_select_form('language_id', $language_options, $_POST['language_id']);
        

        $this->tkeys['local_heading'] = '';
        $this->tkeys['local_author'] = '';
        $this->tkeys['local_contact'] = '';
        $this->tkeys['local_phone'] = '';
        $this->tkeys['local_address'] = '';
        $this->tkeys['local_link'] = '';
        $this->tkeys['local_article'] = '';

		$this->tkeys['captcha_question'] = $publish->craptcha->toString();
		$csrf = new \SFACTIVE\CSRF();
		$this->tkeys['csrf_token'] = $csrf->make_token('publish');

    }

} // end class
?>
