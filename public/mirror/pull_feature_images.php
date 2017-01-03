<?
include("shared/global.cfg");

if (strlen($is_mirror)<1){
echo "WARNING THIS CODE SHOULD ONLY BE RUN ON MIRRORED VERSIONS OF A SITE";
exit;
}

include_once(SF_SHARED_PATH."/classes/category_class.inc");
include_once(SF_SHARED_PATH."/classes/feature_class.inc");

$MIRRORED_SITE_URL="http://sf.indymedia.org";
//$MIRRORED_SITE_URL="http://127.0.0.1";
$db_obj= new DB;

$min_category_id=0;
$min_feature_id=0;

$category_obj= new Category;
$feature_obj= new Feature;

$id_list=$feature_obj->get_id_list(6400,500, "and image like '/%' and image not like '%.php%' ");

while ($next_id_row=array_pop($id_list)){
	$next_id=$next_id_row["id"];
	echo $next_id."<br>";
	$next_feature=$feature_obj->get_feature_fields($next_id);
	$next_upload=$next_feature["image"];
	$file_path=SF_WEB_PATH.$next_upload;
	echo $file_path."<br>";
	if(file_exists($file_path)){
	echo "TRUE<br>";
}else{
	echo "FALSE<BR>";
	$web_path="$MIRRORED_SITE_URL$next_upload";
	echo "$web_path<BR>";
	if($next_handle_read=fopen($web_path,"r")){
	if ($next_handle_write=fopen($file_path,"w")){
	while (!feof($next_handle_read)){
		$next_byte=fread($next_handle_read,1);
		fwrite($next_handle_write, $next_byte);
	}
	fclose($next_handle_write);
	}
	}
}

}

?>