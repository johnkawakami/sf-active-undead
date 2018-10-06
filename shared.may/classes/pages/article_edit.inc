<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';


// Class for article_edit page

class article_edit extends Page {

    function article_edit() {
        // Class constructor, does nothing
        return 1;
    }

    function execute() {
        // Execution method, does nothing
	include_once(SF_CACHE_PATH.'/language_options.inc');
       $tr = new Translate();
       $tr->create_translate_table('article_edit');
       $tr->create_translate_table('publish');

       $cat = new \SFACTIVE\Category();
       $cat->get_category_array();

       $this->tkeys['publish_result'] = "";

       if ($_GET['id'] > 0) $fetch_id = $_GET['id'];

       $type_options = array(
                            "webcast" => "webcast",
                            "news-response" => "news-response",
                            "attachment" => "attachment"
                            );

       $disp_options = array(
                            "l" => $tr->trans('status_local'),
                            "g" => $tr->trans('status_global'),
                            "t" => $tr->trans('status_display'),
                            "f" => $tr->trans('status_hidden')
                            );

       $text_options = array(
                            "t" => "text/plain",
                            "h" => "text/html"
                            );



       $mime_options = array(
                            "application/pdf" =>               "application/pdf",
                            "application/smil" =>              "application/smil",
                            "application/x-shockwave-flash" => "application/x-shockwave-flash",
                            "audio/mpeg" =>                    "audio/mpeg",
                            "audio/x-mpegurl" => "audio/x-mpegurl",
                            "audio/x-ms-wma" => "audio/x-ms-wma",
                            "audio/x-ogg" => "audio/x-ogg",
                            "audio/x-pn-realaudio" =>          "audio/x-pn-realaudio",
                            "audio/x-pn-realaudio-meta" => "audio/x-pn-realaudio-meta",
                            "audio/x-scpls" => "audio/x-scpls",
                            "audio/x-wav" => "audio/x-wav",
                            "image/gif" =>                     "image/gif",
                            "image/jpeg" =>                    "image/jpeg",
                            "image/png" =>                     "image/png",
                            "text/html" =>                     "text/html",
                            "text/plain" =>                    "text/plain",
                            "video/mpeg" =>                    "video/mpeg",
                            "video/quicktime" =>               "video/quicktime",
                            "video/x-ms-wmv" => "video/x-ms-wmv",
                            "video/x-msvideo" => "video/x-msvideo",
                            "video/x-pn-realvideo" =>          "video/x-pn-realvideo",
                            "video/x-pn-realvideo-meta" => "video/x-pn-realvideo-meta"
                            );

// code added by blicero foir the mail2admin4hidnig ftr -- blicero 
	include(SF_INCLUDE_PATH."/select_reason.inc");	


       if (strlen($_POST['editswitch']) > 0) {
	// here we put the script to send a mail to notify the display statu change
	if ($_POST['old_display'] != $_POST['display']) {		
	    $display_msg=$_POST[display];
		$email=$GLOBALS[notifymail];
		$from=$GLOBALS[admin_email];
		$subject=$GLOBALS['dict']['admin_hiding_message'];
		$message=$disp_options[$display_msg];
		$body="post ".$_POST[heading]." id nr ".$_POST[id]."\n".$GLOBALS['dict']['edited_by']." ".$_SESSION[username]."\n".$message."\n".$GLOBALS['dict']['reason'].": ".$_POST[reason]."\n ".$GLOBALS['dict']['additional_reason']." : ".$_POST[additional_reason];
// added control to have this configurable :) -- blicero 22 may 2003
		if ($GLOBALS['notify'] == "1") {
			mail ($email, $subject, $body, "From:$from");
			}
// end ne code :)
	}
       $category_obj = new \SFACTIVE\Category;
	   $db_obj = new \SFACTIVE\DB;
	   $article = new \SFACTIVE\Article($_POST['id']);
	   $article->cleanup_post();

           // we open both newswire and template object
           $newswire_obj = new \SFACTIVE\Newswire();
           $template_obj = new FastTemplate(SF_TEMPLATE_PATH);
	
	   // we here insert an update of the catlink table
	   // we first delete all the entry and then reenter them in the table

           $deletequery = "delete from catlink where id=" . $_POST['id'];
           $executedelete = $db_obj->execute($deletequery);

           // if we have checkboxes we insert into catlink multiple categories
           if (!empty($_POST['catartarray'])) {
               foreach ($_POST['catartarray'] as $catart) {
                   $catupdatequery = "insert into catlink set id=" . $_POST['id'] . ", catid=" . $catart;
                   $executeinsert = $db_obj->execute($catupdatequery);
               }
           }

           $article->update_from_array($_POST);

           // this at the mment refresh all tehnewswires
           // this needs to be changed to only refresh the before or after category array (whichever is greater)
           //$catlist = $cat->get_list();
		$query = "select distinct(c.category_id) as category_id, c.shortname, c.summarylength, c.name, c.newswire from category";
		$query .= " c, catlink l where c.category_id = '".$GLOBALS['config_defcategory']."' or (c.category_id = l.catid and id = '";
		$query .= $_POST['id']."')";
		$catlist = $db_obj->query($query);

           while ($categ = array_pop($catlist))
           {
               $refresh = $newswire_obj->render_summary($categ);
               if($categ['category_id'] !== $GLOBALS['config_defcategory'])
               {
                   $newswire_obj->make_newswire_rss($categ['category_id'],$categ['shortname']);
               }
           }
           
           $newswire_obj->make_newswire_rss();

           // Add a part which refreshes the parent if this is a comment

           $this->update_status("Refreshed ".$_POST['id']);
           $this->tkeys['publish_result'] = $this->get_update_status();

           if ($category) {
               $categorydata=$category_obj->get_category_fields($category1);
               if (is_array($categorydata)>0) $newswire_obj->render_summary($categorydata);
           }

           $categorydata = $category_obj->get_category_fields($GLOBALS["config_defcategory"]);
           if (is_array($categorydata)>0) $newswire_obj->render_summary($categorydata);

           $fetch_id = $_POST['id'];
           $cat->get_article_category_array($_POST['id']);

        } 

        $ar = new \SFACTIVE\Article($fetch_id);
        $s=array('<','>'); $r=array('&lt;','&gt;');
        $this->tkeys['local_summary'] =  str_replace($s,$r,$ar->article['summary']);                
        $this->tkeys['local_article'] =  str_replace($s,$r,$ar->article['article']);
        $this->tkeys['local_file'] =     $ar->article['linked_file'];
        $this->tkeys['local_author'] =   str_replace($s,$r,$ar->article['author']);
        $this->tkeys['local_parentid'] = $ar->article['parent_id'];
        $this->tkeys['local_phone'] =    str_replace($s,$r,$ar->article['phone']);
        $this->tkeys['local_address'] =  str_replace($s,$r,$ar->article['address']);
        $this->tkeys['local_link'] =     str_replace($s,$r,$ar->article['link']);
        $this->tkeys['local_contact'] =  str_replace($s,$r,$ar->article['contact']);
        $this->tkeys['local_title'] =    str_replace($s,$r,$ar->article['heading']);
        $this->tkeys['local_created'] =  $ar->article['format_created'];
        $this->tkeys['local_modified'] = $ar->article['format_modified'];
        $this->tkeys['local_id'] =       $ar->article['id'];
        // added for mail2admin4hidng ftr  -- bliceroi
        $this->tkeys['local_old_display'] =       $ar->article['display'];
        $this->tkeys['select_display'] = $this->make_select_form("display",$disp_options,$ar->article['display']);
        $this->tkeys['select_text'] =    $this->make_select_form("artmime",$text_options,$ar->article['artmime']);
        $this->tkeys['select_mime'] =    $this->make_select_form("mime_type",$mime_options,$ar->article['mime_type']);
        $this->tkeys['select_arttype'] = $this->make_select_form("arttype",$type_options,$ar->article['arttype']);
        $this->tkeys['dropdown_language'] = $this->make_select_form('language_id', $language_options, $ar->article['language_id']);
        // a new select for the hding reason -- blicero        
        $this->tkeys['select_reason'] = $this->make_select_form("reason",$reason_options,$ar->article['reason']);
        // setting the additional reason value -- blicero
        if (!$_POST['additional_reason']) { $this->tkeys['additional_reason_value']=$_POST['additional_reason']; }
		else  { $this->tkeys['additional_reason_value']=""; }
	
        if (!$_POST['id']) $cat->get_article_category_array($ar->article['id']);
        $this->tkeys['local_checkbox'] = $this->make_checkbox_form("catartarray[]",$cat->category_list,$cat->cat_match);
        return 1;
    }


}

