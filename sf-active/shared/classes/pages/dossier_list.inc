<?

class dossier_list extends Page
{

	function dossier_list()
	{
		return 1;
	}

	function execute()
	{
		include_once(SF_CLASS_PATH."/dossier_class.inc");
		$tr = new Translate();
		$tr->create_translate_table('dossier');

		$GLOBALS['page_title'] = $tr->trans('dossier_listpage_title');

		$this->tkeys['dossier_list_link'] = $tr->trans('dossier_list_link') ;
		$this->tkeys['dossier_feature_link'] = $tr->trans('dossier_feature_link');
		$this->tkeys['dossier_article_link'] = $tr->trans('dossier_article_link');
		$this->tkeys['title'] = $tr->trans('dossier_listpage_title');
		$this->tkeys['id'] = $tr->trans('id');
		$this->tkeys['dossier_name'] = $tr->trans('dossier_name');


		$db_obj = new DB();
		$query = $query="select * from feature_dossier order by id_dossier";
		$dossier=$db_obj->query($query);

		$tblhtml = '';

		while($row = array_pop($dossier))
		{
			$tblhtml .= "<tr><td width=\"20\"><a href=\"dossier.php?id=".$row['id_dossier']."\">".$row['id_dossier']."</a></td>";
			$tblhtml .= "<td><a href=\"dossier.php?id=".$row['id_dossier']."\">".$row['name']."</a></td>";
			$tblhtml .= "<td><a href=\"feature.php?id=".$row['id_dossier']."\">".$tr->trans('dossier_edit_index')."</a></td>";
			$tblhtml .= "<td><a href=\"refresh.php?id=".$row['id_dossier']."\">".$tr->trans('dossier_refresh_articles')."</a></td></tr>";
		}

		$this->tkeys['tblhtml'] = $tblhtml ;

	}
}
?>