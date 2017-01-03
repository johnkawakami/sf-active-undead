<?

class dossier_refresh extends Page
{

	function dossier_refresh()
	{
		return 1;
	}

	function execute()
	{
		if(!$_GET['id'])
		{
			$display = false ;
			header('Location: dossier_list.php');
			exit;
		}

		include_once(SF_CLASS_PATH."/newswire_class.inc");
		include_once(SF_CLASS_PATH."/category_class.inc");

		$db_obj = new DB();
		$article_obj = new Article();

		$dossier_id = $_GET['id'];

		$tr = new Translate();
		$tr->create_translate_table('dossier');
		$this->tkeys['dossier_list_link'] = $tr->trans('dossier_list_link');


		//cambio la feature
		//tiro fuori il vecchio sommario e l'id della feature da editare
		$query_dossier="select f.id_feature,feature.summary from feature_dossier f left join feature on feature.feature_id=f.id_feature where f.id_dossier='".$dossier_id."' and feature.is_current_version='1'";
		$result_dossier=$db_obj->query($query_dossier);
		$dossier=array_pop($result_dossier);


		//costruisco l'indice principale
		$query="select w.webcast_id, w.id, w.parent_id, w.order_num, webcast.heading, date_format(webcast.created, '%Y') as year, date_format(webcast.created,'%m') as month from webcast_dossier w left join webcast on webcast.id=w.webcast_id where w.id_dossier='".$dossier_id."' and w.parent_id='0' order by w.order_num desc";

		$body="<!-- non cancellare da qui in giu' quando si edita il post -->\n";
		$res=$db_obj->query($query);
		$body.="<ul>\n";
		while ($art=array_pop($res))
		{
			$body.="<li><a href=\"".SF_NEWS_URL."/".$art[year]."/".$art[month]."/".$art[webcast_id].".php\">".$art[heading]."</a></li>\n";
		}
		$body.="</ul>\n";

		//costruisco il nuovo testo della feature
		$body = addslashes($body);
		$ereg=str_replace("<!-- non cancellare da qui .*","",$dossier[summary]);
		$summary=$ereg.$body;

		//inserisco il nuovo testo della feature
		$insert="update feature set summary='".$summary."' where feature_id='".$dossier[id_feature]."' and is_current_version='1'";
		$yep=$db_obj->execute_statement($insert);

		// cerchiamo tutti gli articoli di un certo dossier e ne aggiorniamo il testo con i link derivati dal loro id
		$query_general="select webcast_id,id from webcast_dossier where id_dossier='".$dossier_id."'";
		$result_general=$db_obj->query($query_general);
		while ($row=array_pop($result_general))
		{
			// recupero i campi dell'articolo
			$article_fields= new Article($row[webcast_id]);

			// recupero i dati degli articoli dipendenti da questo articolo
			$query_loop="select w.webcast_id, w.id, w.parent_id, w.order_num, webcast.heading, date_format(webcast.created, '%Y') as year, date_format(webcast.created,'%m') as month from webcast_dossier w left join webcast on webcast.id=w.webcast_id where w.id_dossier='".$dossier_id."' and w.parent_id='".$row[id]."' order by w.order_num desc";
			$body="<!-- non cancellare da qui in giu' quando si edita il post -->\n";
			$res_loop=$db_obj->query($query_loop);
			$body.="<ul>\n";
			while ($art_loop=array_pop($res_loop))
			{
				$body.="<li><a href=\"".SF_NEWS_URL."/".$art_loop[year]."/".$art_loop[month]."/".$art_loop[webcast_id].".php\">".$art_loop[heading]."</a></li>\n";
			}
			$body.="</ul>\n";

			//ricostruisco il testo dell'articolo con i relativi indici
			$body = preg_replace("/\"/","&quot;",$body);
			$body = addslashes($body);
			$article_fields->article['article']= addslashes($article_fields->article['article']);
			$ereg=ereg_replace("<!-- non cancellare da qui .*","",$article_fields->article['article']);
			$article_fields->article['article'] = $ereg.$body;

			//aggiorno il db
			$update_query="update webcast set article='".$article_fields->article['article']."' where id='".$article_fields->article['id']."'";
			$update_result=$db_obj->execute_statement($update_query);
			//ricacho la pagina
			$id=$article_fields->article[id];
			$article = new Article($id);
			$article->render_everything();
			$article->cache_to_disk();
			$this->tkeys['refreshed'] .= "wrote article $id<br />";
		}
		$this->tkeys['refresh_result'] .= $tr->trans('refresh_result');
		$this->tkeys['work_further'] = $tr->trans('refresh_work_further');
	}
}
?>
