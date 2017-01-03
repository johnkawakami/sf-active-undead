<?php

// Class for feature_display_edit page

class feature_display_edit extends Page
{

    function feature_display_edit()
    {
        return 1;
    }

    function execute()
    {

        include(SF_SHARED_PATH . '/classes/calendar/event_renderer.inc');
        include(SF_SHARED_PATH . '/classes/calendar/date_renderer.inc');
        include(SF_SHARED_PATH . '/classes/calendar/default_object.inc');
	include_once(SF_CACHE_PATH.'/language_options.inc');

        $tr = new Translate();
        $event_renderer = new EventRenderer();
        $date_renderer = new DateRenderer();
	
	$category_id = $_SESSION['category_id'];
	$article_id = $_GET['article_id'];
        $feature_version_id = 0;
        if ($_POST['feature_version_id'] > 0) $feature_version_id = $_POST['feature_version_id'];
        if ($_GET['feature_version_id'] > 0) $feature_version_id = $_GET['feature_version_id'];

        if ($feature_version_id > 0)
        {
            $is_edit = true;
            $this->tkeys['local_subtitle'] = $tr->trans('feature_story_edit');
        } elseif($article_id)
	{
	    $is_promote =  true;
	    $this->tkeys['local_subtitle'] = $tr->trans('feature_story_add');
	}else{
            $is_edit = false;
            $this->tkeys['local_subtitle'] = $tr->trans('feature_story_add');
        }

        $this->tkeys['local_update_rows'] = "";
        $this->tkeys['local_title1'] = "";
        $this->tkeys['local_title2'] = "";
        $this->tkeys['local_order_num'] = "";
        $this->tkeys['local_display_date'] = "";
        $this->tkeys['local_image'] = "";
        $this->tkeys['local_tag'] = "";
        $this->tkeys['local_img_link'] = "";
        $this->tkeys['local_summary'] = "";
	$cur_lang = '';
	if (strlen($_POST['is_submitted'])>0 || strlen($_POST['copy'])>0){
		$this->tkeys['feature_id'] = $_POST['feature_id'];
                $this->tkeys['local_display_date'] = $_POST['display_date'];
		$this->tkeys['modification_date'] = $_POST['mod_date'];
                $this->tkeys['local_tag'] = $_POST['tag'];
                $this->tkeys['local_image'] = $_POST['image'];
                $this->tkeys['version_num'] = $_POST['version_num'];
                $this->tkeys['local_title1'] = $_POST['title1'];
                $this->tkeys['local_title2'] = $_POST['title2'];
                $this->tkeys['creation_date'] = $_POST['creat_date'];
                $this->tkeys['local_order_num'] = $_POST['order_num'];
                $this->tkeys['local_img_link'] = $_POST['image_link'];
                $this->tkeys['local_summary'] = stripslashes($_POST['summary']);
                $this->tkeys['modifier_name'] = $_POST['modifier_name'];
		$template_name=$_POST['template_name'];
		$cur_lang = $_POST['language_id'];
		
	}else{
        	if ($is_edit)
        	{
	        	include(SF_SHARED_PATH . '/classes/feature_class.inc');
	        	$feature_obj = new Feature();
            		$feature = $feature_obj->get_feature_fields($feature_version_id);

            		if (strlen($feature['feature_id']) < 1)
            		{
                		$creation_date = $feature['creat_date'];
                		$modification_date = $feature['mod_date'];
            		}else{
                		$feature_id = $feature['feature_id'];
                		$display_date = htmlentities($feature['display_date']);
                		$creation_date_day = $feature['creation_date_day'];
                		$creation_date_month = $feature['creation_date_month'];
                		$creation_date_year = $feature['creation_date_year'];
                		$modification_date = $feature['mod_date'];
                		$is_current_version = $feature['is_current_version'];
                		$status = $feature['status'];
                		$tag = htmlentities($feature['tag']);
                		$image = $feature['image'];
                		$template_name = $feature['template_name'];
                		$version_num = $feature['version_num'];
                		$title1 = htmlentities($feature['title1']);
                		$title2 = htmlentities($feature['title2']);
                		$creation_date = $feature['creat_date'];
                		$order_num = $feature['order_num'];
                		$image_link = $feature['image_link'];
                		$summary = htmlentities($feature['summary']);
                		$modifier_name = htmlentities($feature['modifier_name']);
				$cur_lang = $feature['language_id'] ;
 		               	$this->tkeys['feature_id'] = $feature['feature_id'];
         		       	$this->tkeys['local_display_date'] = htmlentities($feature['display_date']);
           		     	$this->tkeys['modification_date'] = $feature['mod_date'];
                		$this->tkeys['local_tag'] = htmlentities($feature['tag']);
                		$this->tkeys['local_image'] = $feature['image'];
                		$this->tkeys['version_num'] = $feature['version_num'];
                		$this->tkeys['local_title1'] = htmlentities($feature['title1']);
                		$this->tkeys['local_title2'] = htmlentities($feature['title2']);
                		$this->tkeys['creation_date'] = $feature['creat_date'];
                		$this->tkeys['local_order_num'] = $feature['order_num'];
                		$this->tkeys['local_img_link'] = $feature['image_link'];
                		$this->tkeys['local_summary'] = htmlentities($feature['summary']);
                		$this->tkeys['modifier_name'] = htmlentities($feature['modifier_name']);

                		if (strlen($this->tkeys['modifier_name']) < 1)
                		{
                    			$this->tkeys['modifier_name'] = "---";
                		}
            		}  
		} elseif($is_promote){
			    $art_obj = new Article ;
			    $art_obj->set_article_data($article_id);
                            $feature_id = '';
                            $feature_version_id = '';
			    list($y,$m,$d) = sscanf($art_obj->article['date_entered'], "%d-%d-%d");
			    include_once(SF_CACHE_PATH.'/feature_options.inc');
			    $html = "<tr><td>".$tr->trans('select_a_category')."</td>";
			    $html.= "<td>".$this->make_select_form('category_id', $cat_options, '')."</td></tr>";
			    $this->tkeys['local_update_rows'] = $html;
			    $this->tkeys['local_title1'] = htmlentities($art_obj->article['heading']);
                            $this->tkeys['local_title2'] = htmlentities($art_obj->article['heading']);
			    if($art_obj->article['artmime'] == "h")
			    {
				$this->tkeys['local_summary'] = $art_obj->cleanup_html($art_obj->article['article']);
			    }else{
				$this->tkeys['local_summary'] = $art_obj->cleanup_text($art_obj->article['article']);
			    }
			    if(ereg('image', $art_obj->article['mime_type']))
			    {
				$this->tkeys['local_image'] = $art_obj->article['linked_file'];
			    }
			    $this->tkeys['local_img_link'] = $art_obj->article['link'];
			    $this->tkeys['local_display_date'] = "$m/$d/$y";
			    $cur_lang = $art_obj->article['language_id'];

			    $this->tkeys['local_history_link'] = "";
			    $this->tkeys['local_form_name'] = "feature_add";
			    $this->tkeys['local_form_action'] = "feature_add.php";
			    $this->tkeys['local_form_short_action'] = "add";
			    $this->tkeys['local_copy_link'] = "";
        	} else{
            		$this->tkeys['local_history_link'] = "";
            		$this->tkeys['local_form_name'] = "feature_add";
            		$this->tkeys['local_form_action'] = "feature_add.php";
            		$this->tkeys['local_form_short_action'] = "add";
            		$this->tkeys['local_copy_link'] = "";
            		$this->tkeys['local_display_date'] = date("m/d/Y");

            		if (strlen($template_name) < 1)
            		{
                		$category_obj = new Category;
                		$category = $category_obj->get_category_fields($category_id);
                		$template_name = $category['default_feature_template_name'];
            		}

            		$creation_date_array = getdate(time());
            		$creation_date_day = $creation_date_array['mday'];
            		$creation_date_month = $creation_date_array['mon'];
            		$creation_date_year = $creation_date_array['year'];
        	}
	}
	if ($is_edit){
	    $this->tkeys['local_history_link'] = " | <a href=\"feature_display_history.php?feature_id=";
            $this->tkeys['local_history_link'] .= $feature['feature_id'] . "\">" . $tr->trans('feature_history') . "</A>";

            $this->tkeys['local_form_name'] = "feature_update";
            $this->tkeys['local_form_action'] = "feature_update.php";
            $this->tkeys['local_form_short_action'] = "update";
            $this->tkeys['local_copy_link'] = "&nbsp; &nbsp;<INPUT type=\"submit\" name=\"copy\" value=\"";
            $this->tkeys['local_copy_link'] .= $tr->trans('copy') . "\" />";

            $this->tkeys['local_update_rows'] = "<TR><TD>" . $tr->trans('feature_id_number') . "</TD>";
            $this->tkeys['local_update_rows'] .= "<TD><INPUT type=HIDDEN NAME=\"feature_id\" VALUE=\"";
            $this->tkeys['local_update_rows'] .= $feature['feature_id'] . "\" />" . $feature['feature_id'];
            $this->tkeys['local_update_rows'] .= "</TD></TR><TR><TD>" . $tr->trans('story_version_id') . "</TD>\n";
            $this->tkeys['local_update_rows'] .= "<TD>" . $feature['version_num'] . "</TD><TD>";
            $this->tkeys['local_update_rows'] .= "<INPUT type=HIDDEN NAME=\"version_num\" VALUE=\"";
            $this->tkeys['local_update_rows'] .= $feature['version_num'] . "\" /></TD></TR><TR><TD>";
            $this->tkeys['local_update_rows'] .= $tr->trans('modified') . "</TD><TD>" . $this->tkeys['modifier_name'];
            $this->tkeys['local_update_rows'] .= ", " . $feature['mod_date'] . "</TD></TR>\n";
            
            $this->tkeys['local_display_date'] = $feature['display_date'];	    
	}else{
	    $this->tkeys['local_history_link'] = "";
            $this->tkeys['local_form_name'] = "feature_add";
            $this->tkeys['local_form_action'] = "feature_add.php";
            $this->tkeys['local_form_short_action'] = "add";
            $this->tkeys['local_copy_link'] = "";

            $creation_date_array = getdate(time());
            $creation_date_day = $creation_date_array['mday'];
            $creation_date_month = $creation_date_array['mon'];
            $creation_date_year = $creation_date_array['year'];
	}

        $status_options = array(
                                "c" =>  $tr->trans('current'),
                                "h" =>  $tr->trans('hidden'),
                                "a" =>  $tr->trans('archived')
                                );

        $this->tkeys['local_select_status'] = $this->make_select_form("status", $status_options, $feature['status']);
	// include the array crating the templates from include dir so that everyone can customize it :) --- blicero	
	include(SF_INCLUDE_PATH."/select_templates.inc");

        $this->tkeys['local_select_template'] = $this->make_select_form("template_name", $temp_options, $template_name);

        $this->tkeys['local_select_date'] = "<Select name=\"creation_date_month\">\n";
        $this->tkeys['local_select_date'] .= $event_renderer->create_dropdown($date_renderer->get_months(),$creation_date_month);
        $this->tkeys['local_select_date'] .= "</Select>\n";
        $this->tkeys['local_select_date'] .= "<Select name=\"creation_date_day\">";
        $this->tkeys['local_select_date'] .= $event_renderer->create_dropdown($date_renderer->get_days_of_month(),$creation_date_day);
        $this->tkeys['local_select_date'] .= "</Select>\n";

        $GLOBALS['dropdown_min_year']=$GLOBALS['site_start_year'];

        $this->tkeys['local_select_date'] .= "<Select name=\"creation_date_year\">\n";
        $this->tkeys['local_select_date'] .= $event_renderer->create_dropdown($date_renderer->get_years(),$creation_date_year);
        $this->tkeys['local_select_date'] .= "</Select>\n";

	$language_options[''] = ' '.$tr->trans('select_a_language');
	asort($language_options) ;
	$this->tkeys['local_language'] = $this->make_select_form('language_id', $language_options, $cur_lang);

        return 1;
    }
}

?>
