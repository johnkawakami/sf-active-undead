<?
//This page lists a list of the admin options for the dossier pages

class dossier_admin_index extends Page
{

	function dossier_admin_index()
	{
		return 1;
	}

	function execute()
	{
		include_once(SF_CLASS_PATH."/dossier_class.inc");

		$tr = new Translate();
		$tr->create_translate_table('dossier');
		$tr->create_translate_table('newswire');

		$this->tkeys['sub_title'] = $tr->trans('dossier_adminindex_title');

		$db_obj = new DB();
		$query = "select name, id_dossier from feature_dossier order by id_dossier desc";
		$result = $db_obj->query($query);


		$display_status = array(
								"" =>	$tr->trans('status'),
                                "l" =>  $tr->trans('status_local'),
                                "g" =>  $tr->trans('status_global'),
                                "t" =>  $tr->trans('status_display'),
                                "f" =>  $tr->trans('status_hidden')
                                );




		while($row = array_pop($result))
		{
			$edit_block=$this->make_select_form("value_".$row['id_dossier'], $display_status, "");
			$tblhtml .= "<tr><td width=\"20\"><a href=\"".SF_DOSSIER_URL."/dossier.php?id=".$row['id_dossier']."\">".$row['id_dossier']."</a></td>\n";
			$tblhtml .= "<td><a href=\"".SF_DOSSIER_URL."/dossier.php?id=".$row['id_dossier']."\">".$row['name']."</a></td>\n";
			$tblhtml .= "<td><a href=\"".SF_DOSSIER_URL."/feature.php?id=".$row['id_dossier']."\">".$tr->trans('dossier_edit_index')."</a></td>\n";
			$tblhtml .= "<td><a href=\"".SF_DOSSIER_URL."/refresh.php?id=".$row['id_dossier']."\">".$tr->trans('dossier_refresh_articles')."</a></td>\n";
			$tblhtml .= "<td align=\"center\">".$edit_block."</td> </tr>\n\n";
		}




		$this->tkeys['tblhtml'] = $tblhtml;

	}
}
?>
