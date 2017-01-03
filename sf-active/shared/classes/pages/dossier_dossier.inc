<?

class dossier_dossier extends Page
{

	function dossier_dossier()
	{
		return 1;
	}

	function execute()
	{
		if(!$_GET[id])
		{
			header("Location: index.php");
			exit;
		}
		include_once(SF_CLASS_PATH."/dossier_class.inc");

		$dossier_obj = new Dossier();
		$result_dossier= $dossier_obj->get_dossier($_GET[id]);
		$GLOBALS['current_dossier_name'] =$result_dossier['name'];
		$tr = new Translate();
		$tr->create_translate_table('dossier');

		$GLOBALS['page_title']= $tr->trans('dossier_title');
		$id=$_GET[id];


		$res_max=$dossier_obj->get_max_nesting($_GET[id]);
		$cycle=array_pop($res_max);
		$maxcycle = $cycle[0];

		$db_obj=new DB();
		$query_0="select w.webcast_id, w.id, w.parent_id, w.order_num, webcast.heading, date_format(webcast.created, '%Y') as year, date_format(webcast.created,'%m') as month from webcast_dossier w left join webcast on webcast.id=w.webcast_id where id_dossier='".$id."' and w.parent_id='0' order by w.order_num desc";

		$res_0=$db_obj->query($query_0);

		$result_html = "<ul>";

  		while ($art_0=array_pop($res_0)) {
        	//echo "<li><a href=\"/news/".$art_0[year]."/".$art_0[month]."/".$art_0[webcast_id].".php\">".$art_0[heading]."</a> | <b>id ".$art_0[id]."</b> | <b>order_num ".$art_0[id]."</b> | <a href=\"article.php?id=".$art_0[id]."\">EDITARE</a>";
        	$result_html .= "<li><a href=\"".SF_NEWS_URL."/".$art_0[year]."/".$art_0[month]."/".$art_0[webcast_id].".php\">".$art_0[heading]."</a> | <b>id ".$art_0[id]."</b> | <a href=\"article.php?id=".$art_0[id]."\">".$tr->trans('edit')."</a></li>";
			$result_html .= $this->recursive_generation ($id, $art_0);
		}
		$result_html .= "</ul>";


		if(file_exists(SF_INCLUDE_PATH."/dossier_php.".$GLOBALS['lang'].".inc"))
		{
			$x = file_get_contents(SF_INCLUDE_PATH."/dossier_php.".$GLOBALS['lang'].".inc");

		}else{
			$x = file_get_contents(SF_INCLUDE_PATH."/dossier_php.inc");

		}


	$this->tkeys['dossier_list_link'] = $tr->trans('dossier_list_link');
	$this->tkeys['dossier_article_link'] = $tr->trans('dossier_article_link');
	$this->tkeys['lang_dossier'] = $tr->trans('dossier');
	$this->tkeys['dossier_name'] = $GLOBALS['current_dossier_name'] ;
	$this->tkeys['dossier_title2'] = $result_dossier['title2'];
	$this->tkeys['dossier_summary'] = $summary=ereg_replace("<!-- non cancellare da qui in giu' quando si edita il post -->.*","",$result_dossier['summary']);
	$this->tkeys['articlelist'] = $tr->trans('articlelist');
	$this->tkeys['dossier_list'] = $result_html;
	$this->tkeys['included_file'] = $x;


 }

	function recursive_generation ($id, $art)
	{
		$html = "";
		$tr = new Translate ;
    	$db_obj = new DB;
    	$query="select w.webcast_id, w.id, w.parent_id, w.order_num, webcast.heading, date_format(webcast.created, '%Y') as year, date_format(webcast.created,'%m') as month from webcast_dossier w left join webcast on webcast.id=w.webcast_id where w.id_dossier='".$id."' and w.parent_id='".$art[id]."' order by w.order_num desc";

		$res=$db_obj->query($query);
    	if (!empty($res[0]))
		{
			$html .= "<ul>";
			while ($art=array_pop($res))
			{
            	$html .=  "<li><a href=\"".SF_NEWS_URL."/".$art[year]."/".$art[month]."/".$art[webcast_id].".php\">".$art[heading]."</a> | <b>id ".$art[id]."</b> | <a href=\"article.php?id=".$art[id]."\">".$tr->trans('edit')."</a></li>";
			    $html .= $this->recursive_generation ($id, $art);
			}
	 		$html .=  "</ul>";
	    }
		return $html;
	}

}
?>
