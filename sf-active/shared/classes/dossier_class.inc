<?php

// this class is not an object in itself
// it only contains functions for the dossier interface
// will need to be coded properly sooner or later :(
// blicero -- 2003 sep, 30th

class Dossier
{

function dossier_list () {

	$db_obj = new DB();
	$query="select * from feature_dossier order by id_dossier";
	$dossier = $db_obj->query($query);
	return $dossier;

}

function get_dossier ($id) {

	$db_obj = new DB();
	$query_dossier="select f.name, feature.title2, feature.summary from feature_dossier f left join feature on feature.feature_id = f.id_feature where f.id_dossier='".$id."' and feature.is_current_version='1'";
	$result_dossier=$db_obj->query($query_dossier);
	$result_dossier=array_pop($result_dossier);
	return $result_dossier;
		
}



function get_max_nesting ($id) {

	$db_obj = new DB();
	$query_max="select max(w.nest_level) from webcast_dossier w where id_dossier='".$id."'";
	$res_max=$db_obj->query($query_max);
	return $res_max;

}

function get_feature_dossier_id ($id) {

	$db_obj = new DB();
	$query_select="select id_feature from feature_dossier where id_dossier='".$id."'";
	$result_select=$db_obj->query($query_select);
	$result_select=array_pop($result_select);
	$dossier_id=$result_select[0];
	return $dossier_id;
	
}

function get_feature_dossier_name ($id) {

	$db_obj = new DB();
	$query_select="select name from feature_dossier where id_dossier='".$id."'";
	$result_select=$db_obj->query($query_select);
	$result_select=array_pop($result_select);
	$dossier_name=$result_select[0];
	return $dossier_name;
	
}

function get_feature_dossier_version_id ($id) {

	$db_obj = new DB();
	$query_select="select feature_version_id from feature where feature_id='".$id."' and is_current_version='1'";
	$result_select=$db_obj->query($query_select);
	$result_select=array_pop($result_select);
	$version_id=$result_select[0];
	return $version_id;
	
}

function article_dossier_update ($articlefields,$id) {

        $db_obj = new DB();
	$query="UPDATE webcast SET heading='".$articlefields[heading]."',".
        "author='".$articlefields[author]."',".
        "article='".$articlefields[article]."',".
        "summary='".$articlefields[summary]."',".
        "linked_file='".$articlefields[linked_file]."'";
        $query.=" WHERE id=".$id;
        $db_obj->execute_statement($query);
	return 1;	

}


}

?>