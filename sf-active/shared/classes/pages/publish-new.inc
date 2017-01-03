<?php

// Class for publish page
include_once(SF_SHARED_PATH."/classes/craptcha_class.inc");

class publish extends Page
{
	var $craptcha;

    function publish()
    {
		$key = md5(substr(md5(md5($_SERVER['REMOTE_ADDR'])),4));
	    $this->craptcha = new Craptcha( $key );
        return 1;
    }

    function execute()
    {
	    include_once(SF_SHARED_PATH."/classes/spam_class.inc");
	    $spam=new Spam();

        $spam_publish=0;
        if ($_POST["publish"] != "") $spam_publish=1;

        // $spam_publish is to be checked within the new spam filter - st3

		if ($spam_publish)
		{
			if (!$this->craptcha->answerMatches( $_POST['craptcha'] ))
			{
				echo "Wrong answer to the math question.";
				exit;
			}
		}

        if ( False && $spam->Detect($spam_publish)==1)
        {
			echo "Sorry, no spam.";
            exit;
		}
		// holy fuck, this code is shit


        $comment = false;
        $counter_msg = "";

        $db_obj = new DB();
        $this->tr = new Translate();
        $this->tr->create_translate_table('publish');

        include_once(SF_SHARED_PATH . '/classes/newswire_class.inc');
        include_once(SF_SHARED_PATH . '/classes/category_class.inc');

        // Set up the template variables for this page
        $this->tkeys['upload_max_filesize'] = ini_get('upload_max_filesize');
        $this->tkeys['post_max_size'] = ini_get('post_max_size');
        $this->tkeys['max_execution_time'] = ini_get('max_execution_time')/60;

		$this->tkeys['captcha_question'] = $this->craptcha->toString();
        
        $text_options = array(
                             "t" => "text/plain",
                             "h" => "text/html"
                             );

        $this->tkeys['select_artmime'] = $this->make_select_form("artmime", $text_options, $_POST['artmime']);

        $cat_opt = new Category;


// blicero here we produce the different select for single or multicategorypublishing :)
// removed db-queries. the arrays are now cached to disk when you change categories
// should save 4 till 6 queries per publish.

	if ($GLOBALS['multicat']=="1") 
	{
	    // complete new way. you can now make forms for whatever catclass & more then one if needed
	    // you can also choose between normal select form, checkboxes or multiple-select form.
	
	    // we define the new variable if it doesn't exist so we don't break upgrades
	    if(!$GLOBALS['multicat_forms']) { $GLOBALS['multicat_forms'] = 'tl';}

	    // set the counter, so we now which $_POST['category'][] to point to which form
	    $count = 0;

	    // we include the various array's
	    include_once(SF_CACHE_PATH.'/cattema_options.inc');
	    include_once(SF_CACHE_PATH.'/catlocali_options.inc');
	    include_once(SF_CACHE_PATH.'/catinternal_options.inc');
	    include_once(SF_CACHE_PATH.'/cathidden_options.inc');
	    include_once(SF_CACHE_PATH.'/catevent_options.inc');
	    include_once(SF_CACHE_PATH.'/catproject_options.inc');
	    include_once(SF_CACHE_PATH.'/catother_options.inc');

	    // we start the templating.
	    $dropdown = new FastTemplate(SF_TEMPLATE_PATH.'/pages/');
	    $dropdown->clear_all();
	    $dropdown->define(array(page => "dropdown_multiple_cat.tpl"));
	    $dropdown->define_dynamic("list", "page");
	    
	    $catform = $GLOBALS['multicat_forms'];	    
	    while($char < strlen($catform))
	    {
		switch($catform[$char])
		{
		    // we point to a specific function in cache class that decides on which form to use
		    // (otherwise we had 7 or 8 times a 3 if{}else... structure ...
		    case "t":
			$form = $this->do_form("category[]", $cattema_options, $_POST['category'], $_POST['category']["$count"]);
			$text = $this->tr->trans('sel_cattema');
			break;

		    case "l":
			$form = $this->do_form("category[]", $catlocali_options, $_POST['category'], $_POST['category']["$count"]);
			$text = $this->tr->trans('sel_catlocali');
			break;

		    case "h":
			$form = $this->do_form("category[]", $cathidden_options, $_POST['category'], $_POST['category']["$count"]);
			$text = $this->tr->trans('sel_cathidden');
			break;

		    case "i":
			$form = $this->do_form("category[]", $catinternal_options, $_POST['category'], $_POST['category']["$count"]);
			$text = $this->tr->trans('sel_catinternal');
			break;

		    case "e":
			$form = $this->do_form("category[]", $catevent_options, $_POST['category'], $_POST['category']["$count"]);
			$text = $this->tr->trans('sel_catevent');
			break;

		    case "p":
			$form = $this->do_form("category[]", $catproject_options, $_POST['category'], $_POST['category']["$count"]);
			$text = $this->tr->trans('sel_catproject');
			break;

		    case "o":
			$form = $this->do_form("category[]", $catother_options, $_POST['category'], $_POST['category']["$count"]);
			$text = $this->tr->trans('sel_catother');
			break;
		}
		$dropdown_param["TPL_SEL_CAT"] = $text;
		$dropdown_param["TPL_CAT_SELECT"] = $form;
		$dropdown->assign($dropdown_param);
		$dropdown->parse(LISTS, ".list");
		
		$count ++;
		$char++;
	    }

	    $dropdown->parse(MAIN, "page");
	    $this->tkeys['dropdown_cat'] = $dropdown->fetch("MAIN");
	/*
		// here we put italy code for thematic vs local categories-- blicero
	    //$cattema_options=$cat_opt->get_thematic_assoc_list();
		include_once(SF_CACHE_PATH.'/cattema_options.inc');
        $this->tkeys['cattema_select'] = $this->make_select_form("category[]", $cattema_options, $_POST['category'][0]);
	    
		//$catlocali_options=$cat_opt->get_local_assoc_list();
		include_once(SF_CACHE_PATH.'/catlocali_options.inc');
        $this->tkeys['catlocali_select'] = $this->make_select_form("category[]", $catlocali_options, $_POST['category'][1]);
		// stops here
								
        $dropdown = new FastTemplate(SF_TEMPLATE_PATH."/pages");
        $dropdown->clear_all();
        $dropdown->define(array(page => "dropdown_double_cat.tpl"));
        $dropdown_param=array();
        $dropdown_param["TPL_SEL_CATTEMA"]=$GLOBALS['dict']['sel_cattema'];
        $dropdown_param["TPL_SEL_CATLOCALI"]=$GLOBALS['dict']['sel_catlocali'];
        $dropdown_param["TPL_CATTEMA_SELECT"]=$this->tkeys['cattema_select'];
        $dropdown_param["TPL_CATLOCALI_SELECT"]=$this->tkeys['catlocali_select'];
        $dropdown->assign($dropdown_param);
        $dropdown->parse(CONTENT,"page");
        $this->tkeys['dropdown_cat']=$dropdown->fetch("CONTENT"); */
	}else{
    //$cat_options = $cat_opt->get_newswire_assoc_list();
	include_once(SF_CACHE_PATH.'/cat_options.inc');
    $this->tkeys['category_select'] = $this->make_select_form("category[]", $cat_options, $_POST['category'][0]);
	$dropdown = new FastTemplate(SF_TEMPLATE_PATH."/pages");
	$dropdown->clear_all();
	$dropdown->define(array(page => "dropdown_single_cat.tpl"));
	$dropdown_param=array();
	$dropdown_param["TPL_SEL_CATEGORIES"]=$GLOBALS['dict']['sel_categories'];
	$dropdown_param["TPL_CATEGORY_SELECT"]=$this->tkeys['category_select'];
	$dropdown->assign($dropdown_param);
	$dropdown->parse(CONTENT,"page");
	$this->tkeys['dropdown_cat']=$dropdown->fetch("CONTENT");
	}

       // here we create the dropdown menu for the language selection.
        include_once(SF_CACHE_PATH.'/language_options.inc');
	$language_options[''] = ' '.$this->tr->trans('sel_language') ;
	asort($language_options);
	$this->tkeys['dropdown_language'] = $this->make_select_form('language_id', $language_options, $_POST['language_id']);

        
        $form_vars = array("heading","author","contact","phone","address","summary","link","article");

        // Initialize template variables
        foreach ($form_vars as $fv)
        {
            if (strlen($_POST[$fv]) > 0)
            {
                $this->tkeys['local_'.$fv] = stripslashes($_POST[$fv]);
            } else
            {
                $this->tkeys['local_'.$fv] = '';
            }
        }

        $this->generate_filebox_html();
        $this->tkeys['file_boxes'] = $this->filebox;

        // Set up the file number select list
        $max_num_uploads = array();

        if ($_GET['top_id'] || $_POST['top_id'])
        {
            $max_num_uploads = array(0 => 0, 1 => 1);
        } else
        {
            for ($x = 0; $x <= 20; $x++)
            {
                $max_num_uploads[$x] = $x;
            }
        }

        // Check if we are actually dealing with a comment	
	  if ($_POST['top_id'] > 0)
        {		
            $comment = true;
            $ctr = new Translate();
            $this->forced_template_file = 'comment.tpl';
            $this->tkeys['local_top_id'] = $_POST['top_id'];
            $this->tkeys['pub_stepone'] = $ctr->trans('comment_pub_one');
            $this->tkeys['pub_steptwo'] = $ctr->trans('comment_pub_two');
            $this->tkeys['pub_stepthree'] = $ctr->trans('comment_pub_three');
            $this->tkeys['pub_optional'] = $ctr->trans('optional');
            $this->tkeys['local_site_nick'] = $GLOBALS['site_nick'];
            $this->tkeys['pub_author'] = $ctr->trans('author');
            $this->tkeys['pub_url'] = $ctr->trans('link');
        }

        $this->tkeys['validate_email'] = '';
        $this->tkeys['validate_form'] = '';
        $this->tkeys['validate_checkbox'] = '';

        if($GLOBALS['email_validation'] == "1")
        {
            $this->tkeys['validate_email'] = $this->tr->trans('validate_email');
            $this->tkeys['validate_form'] = $this->tr->trans('validate_form');
            if($_POST['validate']) { $validation_checked = "checked=\"checked\"" ; }
            $this->tkeys['validate_checkbox'] = '<input type="checkbox" name="validate" value="1"  '.$validation_checked.' />';
        }

        $this->tkeys['select_filecount'] = $this->make_select_form("file_count", $max_num_uploads, $_POST['file_count']);

        // First, check if the publish button was actually pressed (as opposed to the upload counter)
        // Also, check if we are supposed to block this IP address

        if (($_POST["publish"] != "") && (sizeof($block_this_ip)==0))
        {

            $article = new Article;
            $article->cleanup_post();

            // Second, if the cleaned-up post is valid, add it to the database
            if ($article->validate_post())
            {

                $db_obj = new DB();
                $newswire = new Newswire();
                $category_obj = new Category();

                $article->process_uploads(); 
                $article->set_article_data_from_form();

                if ($comment)
                {
					$article->article['arttype'] = "news-response";
					$article->article['parent_id'] = $_POST['top_id'];
                }

                $article->insert();

                if (!$comment)
                {
                    $parent_constant = $article->article["id"];
                } else
                {
                    $parent_constant = $article->article['parent_id'];
                }

				$articlelog=new Article($article->article["id"]);
				$spam= new Spam();
				$spam->log_ip($articlelog);
		
                // Third, process the files which were uploaded
                for ($counter = 2; $_POST["file_count"] >= $counter; $counter++)
                {
                    $article->article["artmime"] = 't';
                    if ($_POST["texttype"] == 'text/html' ) $article->article["artmime"] = 'h';
                    $article->article["numcomments"] = 0;
                    $htmlfilename = chop($texttype);

                    $article->article["heading"] =     $_POST["linked_file_title_".$counter];
                    $article->article["article"] =     $_POST["linked_file_comment_".$counter];
                    $article->article["parent_id"] =   $parent_constant;
                    $article->article["mime_type"] =   $_POST["mime_type_file_".$counter];
                    $article->article["summary"] =     "";
                    $article->article["arttype"] =     "attachment";
                    $article->article["html_file"] =   $htmlfilename;
                    $article->article["linked_file"] = $article->upload_target_url[$counter];
                    $article->insert();
                    $counter_msg = "and $counter attachments.";
                }

                // Fourth, add to categories and render the new category newswire
                // The commented out lines allow for checkboxes to be used instead of a dropdown list ...?
// we use an array in any case because in this new system of possible multicategory publishing we parse an array -- blicero (the line for former system can disappear :) 
                if (is_array($_POST["category"]))
                {
                    foreach ($_POST["category"] as $value)
                    {

                        // Next line removed if you use dropdown list
                //        $value = $_POST["category"];

                        if ($value > 0)
                        {
                            $category_obj->add_article_to_category($parent_constant,$value);
                            $categorydata = $category_obj->get_category_fields($value);
                            if (is_array($categorydata) > 0) $Result .= $newswire->render_summary($categorydata);
                            $newswire->make_newswire_rss($value, $categorydata['shortname']);
                        }
                    }
                }

                // Also render the default category
                $cat_data = $category_obj->get_category_fields($GLOBALS["config_defcategory"]);
                $newswire->render_summary($cat_data); 

                // Fifth, cache the page to disk
                $article_obj = new Article($parent_constant);
				
				//echo "step 5<p>";
				//$article_obj->dump_webcast_latest();
				//$article_obj->update_webcast_indexes();
                $article_obj->render_everything();
                $article_obj->cache_to_disk();

                if (ereg('[lgt]',$GLOBALS['newsfeed'])) { $newswire->make_newswire_rss(); }

                if (!$comment)
                {
                    $article_obj->update_status($this->tr->trans('publish_successful'));
                    $this->sf_error_log("Someone has published a new article, id # $parent_constant.");
                } else
                {
                    $article_obj->update_status($this->tr->trans('comment_successful'));
                    $this->sf_error_log("Someone has published a new comment to article # $parent_constant.");
                }
         /*
          * begin jmh/mtoups
          */
       if($GLOBALS['email_validation'] == "1")
       {
         if($_POST["contact"] && $_POST["validate"]){
             /* mailing the validation information to the user */
           /* first we log */
           $this->sf_error_log("entering mail-to-user email validation code, to " . $_POST["contact"]);

             /*constructing the validation url*/
	     $hash = $article_obj->get_validation_hash();
             $validation_url            = SF_ROOT_URL."/publish.php?validate=yes";
             $validation_url            = $validation_url."&hash=". $hash ;
             $validation_url            = $validation_url."&val_id=".$article_obj->article['id'];

             /*sticking the validation url into the email text*/
           $lang_pub_validation_email     = $this->tr->trans('pub_validation_email');
             $lang_pub_validation_email     =  ereg_replace("VALIDATIONURL", $validation_url, $lang_pub_validation_email);
             $lang_pub_validation_email     =  ereg_replace("ADMIN_EMAIL", $GLOBALS['admin_email'], $lang_pub_validation_email);
             $lang_pub_validation_email     =  ereg_replace("PUBLIC_EMAIL", $GLOBALS['public_email'], $lang_pub_validation_email);
             $lang_pub_validation_email     =  ereg_replace("SITE_NAME", $GLOBALS['site_nick'], $lang_pub_validation_email    );

           // article_url added by matt t. 02/09/2003
             $lang_pub_validation_email     =  ereg_replace("ARTICLE_URL", $article_obj->article['article_url_path'] . $article_obj->article['id'] . ".php", $lang_pub_validation_email );

             $lang_pub_validation_email_sub =  $this->tr->trans('pub_validation_email_sub');
             $lang_pub_validation_email_sub =  ereg_replace("SITE_NAME", $GLOBALS['site_nick'], $lang_pub_validation_email_sub);

             /* mailing the user */
             if ( (mail($_POST["contact"],
                   $lang_pub_validation_email_sub,
                   $lang_pub_validation_email,
                   "From: [". $GLOBALS['site_nick'] . "] <" . $GLOBALS['public_email'] . ">\n".
                   "X-Sender: [" . $GLOBALS['site_nick'] . "] <" . $GLOBALS['public_email'] . ">\n".
                   "X-Mailer: PHP (sf-active)\n".
                   "Reply-To: " . $GLOBALS['site_nick'] . "<" . $GLOBALS['public_email'] . ">\n")
               ) != TRUE) {
                   $validation_result = $this->tr->trans('validation_mail_error').'<br />'; //echo "error sending validation mail\n";
               }
               else {
                   $validation_result = $this->tr->trans('validation_email_ok') . '<br />'; //echo "validation email sent!\n";
               }
                  $this->sf_error_log("exiting mail-to-user email validation code");
         } /* end $_POST */
       }else{
           $validation_result = '' ;
       } /* end $_GLOBALS['validate_email']

         /*
          * end jmh
          */

                $Published = true;

                $this->forced_template_file = 'publish_success.tpl';
		$this->tkeys['local_publish_result']  = $validation_result ;
                $this->tkeys['local_publish_result'] .= $article->get_update_status();
                $this->tkeys['local_publish_result'] .= $article_obj->get_update_status();

                if ($comment)
                {
	        include(SF_SHARED_PATH . '/classes/pages/comment_latest.inc');
                $lc = new comment_latest();

		    $comment_link  = $article_obj->article['article_url_path'];
		    $comment_link .= $article_obj->article['id'] . "_comment.php";
		    // set the latest comment page under a global varibale which defines which article display go into the latest comment page -- blicero
                  if (ereg($article_obj->article['display'],$GLOBALS['latest'])) {
			    $lc->add_latest_comment(
                            $article_obj->article['numcomment'],
                            $article_obj->article['heading'],
                            $comment_link,
                            $article_obj->article['id'],
                            stripslashes($article->article['author']),
                            $article->article['id'],
							$article_obj->article['created']
                            );
		    }
                }
            } else
            {
                $this->tkeys['display_preview'] = $article->display_error_status();
            }

            $Instructions = 0;

        } else
        {
        if($GLOBALS['email_validation'] == "1") {
         if($_GET["validate"] == "yes") {
             $Published = true;
             if($_GET["val_id"] > 0){
                     $myarticle = new Article($_GET["val_id"]);
                   $myarticle->set_article_data($_GET["val_id"]);

                   if($myarticle->get_validation_hash() == $_GET["hash"])
                   {
                       // we set the article as validated.
//                       $myarticle->article["validate"] = "validated";
//                       $update_array['id'] = $myarticle->article['id'];
//                       $update_array['validate'] = "validated" ;
//                       $myarticle->update_from_array($update_array);
                       $myarticle->confirm_email_validation(); // this sets things correctly in the db
		        $myarticle->render_everything();
			$myarticle->cache_to_disk();
				       
		       $this->forced_template_file = 'validate.tpl';
		       
                       $this->tkeys['validated'] = $this->tr->trans('validated_ok');
                   }
/*		   elseif($myarticle->article['validate'] == "validated")
                   {
                       $this->forced_template_file = 'validate.tpl';
                       $this->tkeys['validated'] = $this->tr->trans('validated_allready');
		   }*/
		   else{
                       $this->forced_template_file = 'validate.tpl';
                       $this->tkeys['validated'] = $this->tr->trans('validated_error');
                   }
           }else{
               $this->forced_template_file = 'validate.tpl';
                 $this->tkeys['validated'] = $this->tr->trans('validated_error');
             }

        } /* GET[validate] */
       } /* $GLOBALS['validate_email'] */

            // we are displaying the page as normal, no form post
	  if ($_POST["preview"] != "") {
            $preview=1;
	  } else {
            if ($comment==false) $Instructions = 1;
	  }
        }

        if (!$Published && sizeof($block_this_ip)==0)
        {
            foreach ($form_vars as $fv)
            {
                $this->tkeys['local_'.$fv]=htmlspecialchars($this->tkeys['local_'.$fv]);
            }
            //if ($ErrorMessage) echo $ErrorMessage;

            if ($Instructions)
			{
                       $langfile = SF_INCLUDE_PATH . "/publish-instructions" . "." . $GLOBALS['lang'] . ".inc";
                       if ( file_exists($langfile) )
                               {
                                       include($langfile);
                               } else {
                                       include(SF_INCLUDE_PATH . "/publish-instructions.inc");
                        }
                        }

            if ($preview) {
              $article = new Article;
			  $article->cleanup_post();
	      $article->set_article_data_from_form();
	      $article->render();
		  if(!$article->validate_post()) $ErrorMessage = $article->display_error_status();
	
	      $this->tkeys['display_preview'] = stripslashes($article->html_article)."<br />".$ErrorMessage;
	    }
	    else
	    {
		   if(!$this->tkeys['display_preview']) $this->tkeys['display_preview'] = "";
            }
        } else
        {
            //echo $article->cachestatus;
            //echo $article_obj->cachestatus;
        }

    } // end function

    function generate_filebox_html()
    {

        $this->trr = new Translate();
        $this->trr->create_translate_table('publish');

        $fbox = "";
        $fbox = "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"4\" class=\"bgpenult\">\n";
        $fbox .= "<tr><td>";

        if ($_POST['file_count'] > 0)
        {
            $file_count = $_POST['file_count'];
        } else
        {
            $file_count = 0;
        }

        for ($counter = 0; $file_count >= $counter; $counter++)
        {

            if ($counter > 0)
            {
                if ($counter > 1) $fbox .= "<HR />\n";
                $fbox .= "<table width=\"100%\" border=\"0\" cellspacing=\"2\" cellpadding=\"4\"><tr>";
                $fbox .= "<td width=\"25%\" valign=\"top\"><b>";
                $fbox .= $this->trr->trans('select_file') . " #" . $counter . "</b></td>\n";
                $fbox .= "<td width=\"75%\"><input type=\"file\" name=\"linked_file_" . $counter . "\"  /><br />";
                $fbox .= $this->trr->trans('upload_limit') . "</td></tr>\n";

                if ($counter > 1)
                {
                    $fbox .= "<tr><td valign=\"top\" width=\"25%\"><b>" . $this->trr->trans('title') . " #" . $counter . "</b> <small>(";
                    $fbox .= $this->trr->trans('required') . ")</small></td><td>";
                    $fbox .= "<input size=\"25\" maxlength=\"90\" type=\"text\" name=\"linked_file_title_" . $counter;
                    $fbox .= "\" value=\"" . $_POST['linked_file_title_'.$counter] . "\" />";
                    $fbox .= "</td></tr><tr><td valign=\"top\" width=\"25%\">";
                    $fbox .= $this->trr->trans('comment') . " #" . $counter . "</td>\n<td>";
                    $fbox .= "<textarea name=\"linked_file_comment_" . $counter . "\" rows=\"3\" cols=\"50\" wrap=\"virtual\">";
                    $fbox .= $_POST['linked_file_comment_'.$counter] . "</textarea></td></tr>\n";
                }

                $fbox .= "</table>\n";
            }
        }

        $fbox .= "</td></tr></table>\n";
        $this->filebox = $fbox;

    }

} // end class
?>
