<?php

// Class for the Article Mailing Feature

class mailable extends Page {

    function mailable () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
	
		
		// Translate all TPL_ in the Template
		$tr = new Translate();
		$tr->create_translate_table('mailable');
		$this->translation($tr);

// set defauilt values for variables 
		if (isset($_POST[fromaddress])) { $this->tkeys[fromvalue]=$_POST[fromaddress]; } else { $this->tkeys[fromvalue]=""; }
		if (isset($_POST[toaddress])) { $this->tkeys[tovalue]=$_POST[toaddress]; } else { $this->tkeys[tovalue]=""; }
		if (isset($_POST[subject])) { $this->tkeys[subjectvalue]=$_POST[subject]; } else { $this->tkeys[subjectvalue]=""; }

		//if (isset($_POST[commentmail])) { $this->tkeys[commentmail_value]=$_POST[commentmail]; } else { $this->tkeys[commentmail_value]=""; }
		// always blank out the comment - it gets abused for spam
		$this->tkeys['commentmail_value']='';

		if (isset($_POST[id])) { $this->tkeys[idvalue]=$_POST[id]; } elseif (isset($_GET[id])) { $this->tkeys[idvalue]=$_GET[id]; } else { $this->tkeys[idvalue]=""; die('you cannot mail without an article id'); }
		if ($_GET[comments]=="yes") { $this->tkeys[commentsvalue]="yes"; } else { $this->tkeys[commentsvalue]=""; }


// check for mistakes
		if (isset($_POST['email_it'])) {
				// check for captcha
				if ($_POST['captcha']!='4592') { die('sorry'); }
				if (!eregi("^[_a-z0-9-]+(\.[_a-z0-9-]+)*@([0-9a-z](-?[0-9a-z])*\.)+[a-z]{2}([zmuvtg]|fo|me)?$",$_POST[fromaddress])) {  $error_msg.=$tr->trans('from_is_not_mail')."<br>"; }
				if (!eregi("^[_a-z0-9-]+(\.[_a-z0-9-]+)*@([0-9a-z](-?[0-9a-z])*\.)+[a-z]{2}([zmuvtg]|fo|me)?$",$_POST[toaddress])) {  $error_msg.=$tr->trans('to_is_not_mail')."<br>"; }
				if (strlen($_POST[subject]) > 60) {  $error_msg.=$tr->trans('subject_is_too_long')."<br>"; $this->tkeys[subjectvalue]=substr($_POST[subject],0,30);}
				if (strlen($_POST[subject]) < 1 ) {  $error_msg.=$tr->trans('subject_is_null')."<br>"; }
				if (strlen($_POST[commentmail]) > 0) {  $error_msg.=$tr->trans('comment_is_too_long')."<br>"; $this->tkeys[commentmailvalue]=substr($_POST[commentmail],0,300);}

		// see if the recipient is at a valid domain
				$to = $_POST['toaddress'];
				preg_match( '/.+@(.+)$/', $to, $matches );
				$toDomain = $matches[1];
				getmxrr( $toDomain, $exchangers );
				if (! $exchangers)
					$error_msg = '<font color=red>Invalid recipient domain.</font><br>';
		}


// set error_msg value
		if (isset($error_msg)) { $this->tkeys[error_msg]=$error_msg; } else { $this->tkeys[error_msg]=""; }		

// if there is no error then send the mail 	
		if ( (isset($_POST['email_it'])) && (!isset($error_msg)) ) {
			$from="From: ".$_POST['fromaddress']."\r\nReturn-Path: ".$_POST['fromaddress'].
				"\r\nX-Form-User-IP-Address: ".$_SERVER['REMOTE_ADDR'];
			$to=$_POST['toaddress'];
			$subject=$_POST['subject'];
                        
// create the message 
			$article = new Article ($_GET['id']);
                        $article->render();
                        $article->set_paths();
	
			$body.= "\nThis is a message sent from Indymedia.  We have had some trouble\n";
			$body.= "with businesses using the site to send junk email to users.  If this\n";
			$body.= "appears to be capitalist junk mail, please forward it to abuse@slaptech.net\n";
			$body.= "Also, consider making an effort to defame the company in public.\n";
			$body.= "(IP Address of sender: ".$_SERVER['REMOTE_ADDR'].")\n\n";

			$body.=$_POST['commentmail']."\n\n";

			$body.=$tr->trans('prn_original_article').$article->article['article_url']."\n";
			
			// if the article has status of "hidden", truncate the body severely
			if ($article->article['display']=='f')
			{
				$body .= "\nThis article has the status \"hidden\" and has been truncated.\n\n";
				$article->article['article'] = substr($article->article['article'], 0, 100).'...';
				$article->article['summary'] = substr($article->article['summary'], 0, 40).'...';
				echo "sorry, you cannot send hidden messages.";
				exit;
			}

			if (strlen($article->article['linked_file'])>1)
				{
					$body.=$tr->trans('original_upload').$article->article['fileurl'];
					$body.=" (".$article->article['filesize'].")\n\n";
				} else {
					$body.="\n\n";
				}
			$body.=$article->article['heading']; // ."\n";
			if (substr($article->article['heading'],-1,1)!="\n") $body.="\n";
			$body.=$article->article['author'];
			if (strlen($article->article['contact'])>1)
				{
					$body.=" - ".$article->article['contact']."\n";
				}
			if (strlen($article->article['address'])>1)
				{
					$body.=$article->article['address']." ";
				}
			if (strlen($article->article['phone'])>1)
				{
					$body.=$article->article['phone'];
				}
			if (strlen($article->article['address'])>1 || strlen($article->article['phone']!="")>1)
				{
					$body.="\n";
				}
			$body.=$article->article['format_created']."\n\n";
			$body.=eregi_replace("<br />","\n",$article->article['summary'])."\n\n";
			$body.=eregi_replace("<br />","\n",$article->article['article'])."\n\n";
			$body.=$article->article['link']."\n\n\n";

			if ( ( $_GET['comments'] == "yes" ) || ($_POST['comments'] == "yes" ) ) {
		
			if ( $article->article['numcomment'] > 0 ) 
				{
					$body.=$tr->trans('plural_comment').":\n\n";
					$article->set_comments_data("asc",0);
					if (is_array($article->comments))
					{
					    while($article_fields = array_pop($article->comments))
					    {
						$comment = new Article($article_fields["id"]);
						$body.=$comment->article['heading'];
						if (substr($comment->article['heading'],-1,1)!="\n") $body.="\n";
						$body.=$comment->article['author'];
						if (strlen($comment->article['contact'])>1)
						    {
							$body.=" - ".$comment->article['contact']."\n";
						    }
						if (strlen($comment->article['address'])>1)
						    {
							$body.=$comment->article['address']." ";
						    }
							if (strlen($comment->article['phone'])>1)
						    {
							$body.=$comment->article['phone'];
						    }
							if (strlen($comment->article['address'])>1 || strlen($comment->article['phone'])>1)
						    {
							$body.="\n";
						    }
						$body.=$comment->article['format_created']."\n";
						if (strlen($comment->article['linked_file'])>1)
						    {
							$body.=$tr->trans('original_upload').$comment->article['fileurl'];
							$body.=" (".$comment->article['filesize'].")\n\n";
						    } else {
							$body.="\n\n";
						    }
						$body.=eregi_replace("<br />","\n",$comment->article['article'])."\n\n";
						$body.=$article->article['link']."\n\n\n";
					    }
					}

				} else {
				
					$body .= $tr->trans('prn_no_comments');
					
				}
			}

			$body=strip_tags($body,"<a><i><b><u><br><p><h1><h2><h3><h4><h5><h6><hr>");
//			$body=eregi_replace("<a href=\"",$tr->trans("link").": ",$body);
//			$body=eregi_replace("\">"," >",$body);
//			$body=eregi_replace("</a>","< ",$body);

//			$body=eregi_replace('<A .*HREF=("|\')?([^ "\']*)("|\')?.*>([^<]*)</A>', '[\\4] ('.$tr->trans("link").': \\2)',$body);

//			$body=eregi_replace('(<a [^<]*href=["|\']?([^"\']*)["|\']?[^>].*>([^<]*)</a>)','[\\3] ('.$tr->trans("link").': \\2)', $body);

			$body=eregi_replace("<i>","/",$body);
			$body=eregi_replace("</i>","/",$body); 
			$body=eregi_replace("<b>","*",$body);
			$body=eregi_replace("</b>","*",$body);
			$body=eregi_replace("<u>","_",$body);
			$body=eregi_replace("</u>","_",$body);
			$body=eregi_replace("<br>","\n",$body);
			$body=eregi_replace("<p>","\n\n\t",$body);
			$body=eregi_replace("</p>","\n\n",$body);
			$body=eregi_replace("<hr>","\n----\n",$body);
			$body=eregi_replace("<h?>","\n\n",$body);
			$body=eregi_replace("</h?>","\n\n",$body);

			$body=eregi_replace('(<a [^<]*href=["|\']?([^"\']*)["|\']?[^>].*>([^<]*)</a>)','[\\3] ('.$tr->trans("link").': \\2)', $body);


			$body.="\n\n".$tr->trans('notice').SF_ROOT_URL."/process/disclaimer.php - ".SF_ROOT_URL."/process/privacy.php";

		// send the mail
			mail($to,$subject,$body,$from);	

		header("Location: ".$article->article['article_url']);
		}	

		
		if ( $_GET['id'] > 0 ) 
		{

			$article = new Article ($_GET['id']);
			$article->render();
			$article->set_paths();

			$this->tkeys['PRN_ARTICLE_HTML'] = $article->html_article;
			$this->tkeys['PRN_ARTICLE_URL'] = $article->article['article_url'];
			$this->tkeys['PRN_TITLE'] = $article->article['heading']." : ".$GLOBALS['site_nick'];

			//$PHP_SELF."?id=".$_GET['id'];	
			//$PHP_SELF."?id=".$_GET['id']."&comments=yes";

			if ( $_GET['comments'] == "yes" ) 
			{
				
				$this->tkeys ['PRN_COMMENT_LINK'] = $PHPSELF."?id=".$_GET['id'];
				$this->tkeys ['PRN_COMMENT_TEXT'] = $tr->trans('prn_hide_comments');
				
				if ( $article->article['numcomment'] > 0 ) 
				{
					$article->render_all_comments();
					$this->tkeys ['PRN_COMMENTS_HTML'] = $article->html_comments;
					
				} else {
				
					$this->tkeys ['PRN_COMMENTS_HTML'] = $tr->trans('prn_no_comments');
					
				}
	
			} else {
	
				$this->tkeys ['PRN_COMMENT_LINK'] = $PHPSELF."?id=".$_GET['id']."&comments=yes";
				$this->tkeys ['PRN_COMMENT_TEXT'] = $tr->trans('prn_print_comments');
				$this->tkeys ['PRN_COMMENTS_HTML'] = "";
				
	
			}

			
		} else {
			$this->tkeys ['PRN_COMMENTS_HTML'] = "";
			$this->tkeys['PRN_TITLE'] = "";
			$this->tkeys['PRN_ARTICLE_URL'] = "";
			$this->tkeys ['PRN_COMMENT_LINK'] = "";
			$this->tkeys ['PRN_COMMENT_TEXT'] = "";
			$this->tkeys ['PRN_ARTICLE_HTML'] = $tr->trans('prn_no_id');
		}
		
		return 1;		
	}

	function translation ($tr) {
		$this->tkeys['LANG_PRN_ORGINAL_ARTICLE'] = $tr->trans('prn_orginal_article');
		$this->tkeys['LANG_PRN_SITE_NAME'] = $GLOBALS['site_name'];
	}

}
