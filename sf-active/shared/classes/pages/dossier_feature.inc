<?

class dossier_feature extends Page
{
	function dossier_feature()
	{
		return 1;
	}

	function execute()
	{
		include_once(SF_CLASS_PATH."/dossier_class.inc");
		include_once(SF_CLASS_PATH . '/calendar/event_renderer.inc');
		include_once(SF_CLASS_PATH . '/calendar/date_renderer.inc');
		include_once(SF_CLASS_PATH . '/calendar/default_object.inc');
		include_once(SF_CACHE_PATH . '/language_options.inc');

		$tr = new Translate();
		$tr->create_translate_table('dossier');
		$tr->create_translate_table('feature');

		$db_obj = new DB();
		$event_renderer = new EventRenderer();
		$date_renderer = new DateRenderer();
		$dossier_obj = new dossier();


		$default_category = $GLOBALS['config_defcategory'];
		$id = $_GET['id'];

		$dossier_test=$dossier_obj->get_feature_dossier_id ($id);

		if($id)
		{
			$is_edit = true ;
			$dossier_feature_id=$dossier_obj->get_feature_dossier_id($id);
			$dossier_name= $dossier_obj->get_feature_dossier_name($id);
			$feature_version_id = $dossier_obj->get_feature_dossier_version_id ($dossier_feature_id);
			$GLOBALS['page_title'] = $tr->trans('dossier_feature_edittitle'). " ".$dossier_name;
			$this->tkeys['sub_title'] = $tr->trans('dossier_feature_edittitle'). " ".$dossier_name;
		}else{
			$is_edit=false;
			$GLOBALS['page_title'] = $tr->trans('dossier_feature_newtitle');
			$this->tkeys['sub_title'] = $tr->trans('dossier_feature_newtitle');
		}

		if ($is_edit)
		{
			include_once(SF_CLASS_PATH."/feature_class.inc");
			$this->tkeys['local_form_name'] = "feature_update";
			$this->tkeys['local_form_action'] = "feature_update.php";
			$this->tkeys['local_form_short_action'] = "update";
			$feature_obj= new Feature();
			$feature=$feature_obj->get_feature_fields($feature_version_id);
			if (strlen($feature_id)>0)
			{
				$this->tkeys['creation_date']=$feature[create_date];
				$this->tkeys['modification_date']=$feature[mod_date];
			}else{
				$feature_id=$feature[feature_id];
				$this->tkeys['local_display_date'] = htmlentities($feature[display_date]);
				$creation_date_day=$feature[creation_date_day];
				$creation_date_month=$feature[creation_date_month];
				$creation_date_year=$feature[creation_date_year];
				$modification_date=$feature[mod_date];
				$is_current_version=$feature[is_current_version];
				$status=$feature[status];
				$tag=htmlentities($feature[tag]);
				$image=$feature[image];
				$template_name=$feature[template_name];
				$version_num=$feature[version_num];
				$cur_lang = $feature['language_id'];
				$this->tkeys['local_title1']=htmlentities($feature[title1]);
				$this->tkeys['local_title2']=htmlentities($feature[title2]);
				$creation_date=$feature[creat_date];
				$order_num=$feature[order_num];
				$image_link=$feature[image_link];
				$this->tkeys['local_summary']=htmlentities($feature[summary]);
				$modifier_name=htmlentities($feature[modifier_name]);
				if (strlen($modifier_name)<1)
				{
					$modifier_name="sconosciuto";
				}

				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"feature_id\" value=\"".$feature_id."\" />\n";
				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"feature_version_id\" value=\"".$feature_version_id."\" />\n";
				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"version_num\" value=\"".$version_num."\" />\n";
				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"dossier_name\" value=\"".$dossier_name."\" />\n";
				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"status\" value=\"".$status."\" />\n";
				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"category_id\" value=\"".$feature['category_id']."\" />\n";
				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"order_num\" value=\"".$order_num."\" />\n";
				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"template_name\" value=\"".$template_name."\" />\n";
				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"image\" value=\"".$image."\" />\n";
				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"image_link\" value=\"".$image_link."\" />\n";
				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"tag\" value=\"".$tag."\" />\n";
				$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"modifier_name\" value=\"".$modifier_name."\" />\n";


				$this->tkeys['local_update_rows'] .= "<tr><td>".$tr->trans('feature_id_number')."</td><td>".$feature_id."</td></tr>";
				$this->tkeys['local_update_rows'] .= "<tr><td>".$tr->trans('story_version_id')."</td><td>".$feature_version_id."</td></tr>";
				$this->tkeys['local_update_rows'] .= "<tr><td>".$tr->trans('feature_version_num')."</td><td>".$version_num."</td></tr>";
				$this->tkeys['local_update_rows'] .= "<tr><td>".$tr->trans('dossier_name')."</td><td>".$dossier_name."</td></tr>";
			}
		}else{
			$this->tkeys['local_form_name'] = "feature_add";
			$this->tkeys['local_form_action'] = "feature_add.php";
			$this->tkeys['local_form_short_action'] = "add";
			$this->tkeys['local_update_rows'] = "";
			$this->tkeys['local_title1'] = "";
			$this->tkeys['local_title2'] = "";
			$this->tkeys['local_display_date'] = "";
			$this->tkeys['local_summary'] = "";
			$cur_lang = '';
			$creation_date_array = getdate(time());
            $creation_date_day = $creation_date_array['mday'];
            $creation_date_month = $creation_date_array['mon'];
            $creation_date_year = $creation_date_array['year'];

			// categories
			$dossier_cats =$GLOBALS['dossier_cat_display'];
			while ($char < strlen($dossier_cats))
			{
				$display_query .= "catclass='$dossier_cats[$char]' or "; $char++;
			}
			$display_query = substr($display_query,0,strlen($display_query)-3);
			$sql = "select category_id, name from category where ".$display_query." order by order_num desc";
			$cats = $db_obj->query($sql);
			$cat_box = "<select name=\"category_id\">";
			while($row = array_pop($cats))
			{
				$cat_box .= "<option value=\"".$row['category_id']."\">".$row['name']."</option>";
			}
			$cat_box .= "</select>";


			$this->tkeys['local_hidden_rows'] = "<input type=\"hidden\" name=\"status\" value=\"h\" />\n";
			$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"order_num\" value=\"0\" />\n";
			$this->tkeys['local_hidden_rows'] .= "<input type=\"hidden\" name=\"template_name\" value=\"title_no_image_row\" />\n";

			$this->tkeys['local_update_rows'] .= "<tr><td><b>".$tr->trans('dossier_name')."</b></td><td><input type=\"text\" name=\"dossier_name\" value=\"\" /></td></tr>\n";
			$this->tkeys['local_update_rows'] .= "<tr><td><b>".$tr->trans('select_a_category')."</b></td><td>".$cat_box."</td></tr>\n";

		}
        $this->tkeys['local_select_date'] = "<select name=\"creation_date_month\">\n";
        $this->tkeys['local_select_date'] .= $event_renderer->create_dropdown($date_renderer->get_months(),$creation_date_month);
        $this->tkeys['local_select_date'] .= "</select>\n";
        $this->tkeys['local_select_date'] .= "<select name=\"creation_date_day\">";
        $this->tkeys['local_select_date'] .= $event_renderer->create_dropdown($date_renderer->get_days_of_month(),$creation_date_day);
        $this->tkeys['local_select_date'] .= "</select>\n";

        $GLOBALS['dropdown_min_year']=$GLOBALS['site_start_year'];

        $this->tkeys['local_select_date'] .= "<select name=\"creation_date_year\">\n";
        $this->tkeys['local_select_date'] .= $event_renderer->create_dropdown($date_renderer->get_years(),$creation_date_year);
        $this->tkeys['local_select_date'] .= "</select>\n";

	$language_options[''] = ' '.$tr->trans('select_a_language');
	asort($language_options);
	$this->tkeys['local_language'] = $this->make_select_form('language_id', $language_options, $cur_lang);

	}

}
?>
