<?

class dossier_article extends Page
{

	function dossier_article()
	{
		return 1;
	}

	function execute()
	{
		include_once(SF_CLASS_PATH."/category_class.inc");
		include_once(SF_CLASS_PATH."/newswire_class.inc");
		include_once(SF_CLASS_PATH."/dossier_class.inc");

		include_once(SF_SHARED_PATH."/classes/spam_class.inc");
		$spam=new Spam();
        $spam_publish=0;
        if ($_POST["publish"] != "") $spam_publish=1;
        if ($spam->Detect($spam_publish)==1)
        {
		    $tr = new Translate;
            echo "<h3>".$tr->trans('spam_message')."<h3>";
            echo "</body></html>";
	    exit;
		}

		$db_obj = new DB;
		$tr = new Translate ;
		$tr->create_translate_table('dossier');
		$tr->create_translate_table('publish');
		$this->tkeys['dossier_list_link'] = $tr->trans('dossier_list_link');
		$this->tkeys['dossier_article_link'] = $tr->trans('dossier_article_link');
		$this->tkeys['lang_dossier'] = $tr->trans('dossier');
		$this->tkeys['dossier_name'] = $GLOBALS['current_dossier_name'] ;
	        // here we create the info for the dropdown menu for the language selection.
		include_once(SF_CACHE_PATH.'/language_options.inc');
	        $language_options[] = " ".$tr->trans('sel_language') ;
		asort($language_options);
		$cur_lang = $_POST['language_id'] ;
		//$this->tkeys['dropdown_language'] = $this->make_select_form('language', $language_options, $_POST['language']);



		if ($_POST[edit]) {

			//aggiorna l'articolo
			//non uso update_article perche' alcuni valori non sono contemplati dalla form del dossier
			$dossier_obj = new Dossier();
			$update= $dossier_obj->article_dossier_update($_POST,$_POST[article_id]);
			//prepariamo nest_level
			if ($_POST[parent_id_dossier] == "0") {
				$nest_level="0";
			}else {
				$nest_parent=$_POST[parent_id_dossier];
				$query="select nest_level from webcast_dossier where id='".$nest_parent."'";
				$res_nest=$db_obj->query($query);
				$res_nest=array_pop($res_nest);
				$nest_level=$res_nest[nest_level];
				$nest_level++;
			}

			// aggiorniamo in webcast_dossier
			$query2="UPDATE webcast_dossier SET parent_id='".$_POST[parent_id_dossier]."', order_num='".$_POST[order_num]."', nest_level='".$nest_level."'";
			$query2.=" WHERE webcast_id=".$_POST[article_id];
			$db_obj->execute_statement($query2);

			//manca la capacita' di cachare l'articolo dopo averlo aggiornato
			//devo finire :)))))
			$_POST['id'] = $_POST[article_id];
			$article_obj=new Article($_POST['id']);
			$article_obj->cleanup_post();
			$article_obj->validate_post();
			if($article_obj->validate_post())
			{
			    $article_obj->update_from_array($_POST);
			    $this->forced_template_file = 'publish_success.tpl';
			    $this->tkeys['local_publish_result'] = "<br /><br /><b>".$tr->trans('update_ok').":</b> ".$id."<br /><b><a href=\"dossier_list.php\">".$tr->trans('dossier_list_link')."</a></b>";
			    $this->tkeys['status'] = $status;
			}else{
			    $this->tkeys['status'] = $article_obj->display_error_status();
			    $this->tkeys['hiddensettings'] = "<input type=\"hidden\" name=\"article_id\" value=\"$_POST[id]\" />";
			    $this->tkeys['hiddensettings'] .= "<input type=\"hidden\" name=\"edit\" value=\"edit\"  />";
			}
			//exit;
		}

		if ($_GET['id'])
		{
			//recuperiamo le informazioni dal db per l'articolo e le printiamo nel simpatico form :))))
			$query = "select * from webcast_dossier where id='".$_GET['id']."'";
			$doit = $db_obj->query($query);
			$dossier_info = array_pop($doit);
			$article_fields = new Article($dossier_info[webcast_id]);
			$this->tkeys['local_author'] = $article_fields->article['author'];
			$this->tkeys['local_heading'] = $article_fields->article['heading'];
			$this->tkeys['local_article'] = $article_fields->article['article'];
			$this->tkeys['local_summary'] = $article_fields->article['summary'];
			$dos_select=$dossier_info[id_dossier];
			$this->tkeys['local_parent_id']=$dossier_info[parent_id];
			$this->tkeys['local_order_num']=$dossier_info[order_num];
			$this->tkeys['hiddensettings'] = "<input type=\"hidden\" name=\"edit\" value=\"edit\"  />";
			$this->tkeys['hiddensettings'] .= "<input type=\"hidden\" name=\"article_id\" value=\"".$dossier_info['webcast_id']."\"  />";
			$cur_lang = $article_fields->article['language_id'];
		}

		if($_POST){
			$this->tkeys['local_author'] = $_POST['author'];
			$this->tkeys['local_heading'] = $_POST['heading'];
			$this->tkeys['local_order_num'] = $_POST['order_num'];
			$this->tkeys['local_parent_id'] = $_POST['parent_id_dossier'];
			$this->tkeys['local_article'] = $_POST['article'];
			$this->tkeys['local_summary'] = $_POST['summary'];
			$dos_select = $_POST['id_dossier'];
		}elseif($_GET){
			$this->tkeys['status'] = "";
		}else{
			$this->tkeys['status'] = "";
			$this->tkeys['local_author'] = "";
			$this->tkeys['local_heading'] = "";
			$this->tkeys['local_order_num'] = "";
			$this->tkeys['local_parent_id'] = "";
			$this->tkeys['local_article'] = "";
			$this->tkeys['local_summary'] = "";
			$this->tkeys['hiddensettings'] = "";
		}

		$this->tkeys['sub_title'] = $tr->trans('dossier_article_add_title');
		$this->tkeys['upload_max_filesize'] = ini_get('upload_max_filesize');
		$this->tkeys['post_max_size'] = ini_get('post_max_size');
		$this->tkeys['max_execution_time'] = ini_get('max_execution_time')/60;
		$this->tkeys['dropdown_language'] = $this->make_select_form('language_id', $language_options, $cur_lang);

		$sql = "select name, id_dossier from feature_dossier";
		$result = $db_obj->query($sql) ;
		$return_array[0] = $tr->trans('dossier_select');

        while ($row = array_pop($result))
        {
            $key = $row['id_dossier'];
	   		$return_array[$key] = $row['name'];
        }
        $dropdown = $this->make_select_form("id_dossier", $return_array, $dos_select);
		$this->tkeys['local_select_a_dossier'] = "<td width=\"25%\">".$tr->trans('dossier_select')."</td><td width=\"75%\">".$dropdown."</td>";

		$this->generate_filebox_html();
        $this->tkeys['file_boxes'] = $this->filebox;
		$max_num_uploads = array(0 => 0, 1 => 1);
		$this->tkeys['select_filecount'] = $this->make_select_form("file_count", $max_num_uploads, $_POST['file_count']);



		if ($_POST[publish] != "" && !$_POST['edit'])
		{
			$article = new Article;
            $article->cleanup_post();
			if($article->validate_post())
			{

            // Second, if the cleaned-up post is valid, add it to the database
                $db_obj = new DB();
                $newswire = new Newswire();
                $category_obj = new Category();

                $article->process_uploads();
                $article->set_article_data_from_form();

				$article->article['display'] = 'f';
				$article->article['parent_id'] = 0;
				$this->article['created_year'] = date(Y);
				$this->article['created_month'] = date(m);
                $article->insert();

                    $parent_constant = $article->article['id'];

		$articlelog=new Article($article->article["id"]);
		$spam= new Spam();
		$spam->log_ip($articlelog);

                $article_obj = new Article($parent_constant);
                $article_obj->render_everything();
                $article_obj->cache_to_disk();

                $this->forced_template_file = 'publish_success.tpl';
                $this->tkeys['local_publish_result']  = $article->get_update_status();
                $this->tkeys['local_publish_result'] .= $article_obj->get_update_status();


			$data['id_dossier'] = $_POST['id_dossier'];
			$data['parent_id_dossier'] = $_POST['parent_id_dossier'];
			$data['order_num'] = $_POST['order_num'];
			$data['webcast_id'] = $article->article['id'];

			if ($_POST['parent_id_dossier'] == "0")
			{
				$nest_level="0";
			}else {
				$nest_parent=$_POST['parent_id_dossier'];
				$query="select nest_level from webcast_dossier where id='".$nest_parent."'";
				$res_nest=$db_obj->query($query);
				$res_nest=array_pop($res_nest);
				$nest_level=$res_nest[nest_level];
				$nest_level++;
			}

			$query_dossier="insert into webcast_dossier values('', '".$data[webcast_id]."', '".$data[id_dossier]."', '".$data[parent_id_dossier]."', '".$data[order_num]."', '".$nest_level."')";
			$exe=$db_obj->execute_statement($query_dossier);

			//category
			//innanzitutto determina la cat_id a partire dal dossier_id
			$query_cat = "select cat_id from feature_dossier where id_dossier='".$data[id_dossier]."'";
			$result_cat = $db_obj->query($query_cat);
			$row = array_pop($result_cat);
			$value= $row[cat_id];
			if ($value)
			{
				$query = "select max(id) from webcast where heading='$heading'";
    			$result = $db_obj->query($query);

				if ($result)
				{
                	$row=array_pop($result);
					$categoryid = $row[0];
     				$query2 = "INSERT INTO catlink VALUES($value,$categoryid)";
					$result = $db_obj->execute_statement($query2);
				}
			}
			$this->forced_template_file = 'publish_success.tpl';
			$this->tkeys['local_publish_result'] = "<br /><br /><b>".$tr->trans('publish_successful').":</b> ".$article->article['id']."<br />";
			$this->tkeys['local_publish_result'] .= $tr->trans('dossier_publish_ok');
			$this->tkeys['local_publish_result'] .= "<br /><a href=\"".SF_DOSSIER_URL."/refresh.php?id=".$data['id_dossier']."\">".$tr->trans('dossier_refresh_articles')."</a> - ";
			$this->tkeys['local_publish_result'] .= "<a href=\"".SF_DOSSIER_URL."/\">".$tr->trans('dossier_list_link')."</a>";
		}else{
			$this->tkeys['status'] = $article->display_error_status();
		}
		}}



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

}


?>
