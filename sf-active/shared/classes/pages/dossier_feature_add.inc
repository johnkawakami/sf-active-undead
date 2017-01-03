<?

//This file adds submitted stories to the DB

class dossier_feature_add extends Page
{

	function dossier_feature_add()
	{
		return 1;
	}

	function execute()
	{
		$display = false ;
		include_once(SF_CLASS_PATH."/feature_class.inc");
		$db_obj = new DB();
		$feature_obj = new Feature();


		$featurefields=array(
			title1=>$_POST['title1'],
			title2=>$_POST['title2'],
			display_date=>$_POST['display_date'],
			creation_date=>$_POST['creation_date_year']."-".$_POST['creation_date_month']."-".$_POST['creation_date_day']." 00:00:00",
			summary=>$_POST['summary'],
			order_num=>$_POST['order_num'],
			status=>$_POST['status'],
			tag=>$_POST['tag'],
			image=>$_POST['image'],
			image_link=>$_POST['image_link'],
			category_id=>$_POST['category_id'],
			template_name=>$_POST['template_name'],
			language_id=>$_POST['language_id'],
		);
		$error_num=$feature_obj->add($featurefields);

		//inoltre aggiorno il mio db
		$query = "select feature_id,category_id from feature where title2='".$_POST['title2']."'";
		$result = $db_obj->query($query);
		$feature_dossier = array_pop($result);
		$query2 = "insert into feature_dossier set id_feature='".$feature_dossier[feature_id]."', cat_id='".$feature_dossier[category_id]."', name='".$_POST['dossier_name']."'";
		$act=$db_obj->execute_statement($query2);

		if ($error_num==0)
		{
			header("Location: /dossier/index.php");
		}else{
			$tr = new Translate ;
			$tr->create_translate_table('dossier');
			$error_message = $tr->trans('dossier_addfeature_error');
			$error_message .= "<br /><a href=\"feature.php\">".$tr->trans('try_again')."</a>";
			echo $error_message;
		}
	}
}
?>
