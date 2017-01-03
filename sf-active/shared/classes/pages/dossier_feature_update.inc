<?

class dossier_feature_update extends Page
{

	function dossier_feature_update()
	{
		return 1;
	}

	function execute()
	{
		$display=false;
		include_once(SF_CLASS_PATH."/dossier_class.inc");
		include_once(SF_CLASS_PATH."/feature_class.inc");

		$db_obj=new DB();
		$feature_obj=new Feature;
		$user_id='0';

		if($_POST){
		$featurefields=array(
			feature_version_id=>$_POST['feature_version_id'],
			feature_id=>$_POST['feature_id'],
			title1=>$_POST['title1'],
			title2=>$_POST['title2'],
			display_date=>$_POST['display_date'],
			creation_date=>$_POST['creation_date_year']."-".$_POST['creation_date_month']."-".$_POST['creation_date_day']." 00:00:00",
			summary=>$_POST['summary'],
			order_num=>$_POST['order_num'],
			status=>$_POST['status'],
			tag=>$_POST['tag'],
			image_link=>$_POST['image_link'],
			template_name=>$_POST['template_name'],
			image=>$_POST['image'],
			category_id=>$_POST['category_id'],
			language_id => $_POST['language_id'],
		);
		$error_num=$feature_obj->update($featurefields);
		}
		else{
			$error_num = true ;
		}
		if ($error_num==0)
		{
			header('Location: /dossier/');
		}else{
			$tr = new Translate ;
                        $tr->create_translate_table('dossier');
                        $error_message = $tr->trans('dossier_updatefeature_error');
                        $error_message .= "<br /><a href=\"feature.php\">".$tr->trans('try_again')."</a>";
                        echo $error_message;

		}
	}
}
?>
