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

$feature_url=$MIRRORED_SITE_URL."/mirror/feature_dump.php";

$db_obj= new DB;

$min_category_id=0;
$min_feature_id=0;

$category_obj= new Category;
$feature_obj= new Feature;
$min_category_id=$category_obj->get_max_id();
$min_feature_id=$feature_obj->get_max_id();

$feature_url.="?min_category_id=$min_category_id&min_feature_id=$min_feature_id&num_rows=500";
$file_handle=fopen($feature_url,"r");
$i=0;
while (!feof($file_handle) && i<5){
	$nextchar=fread($file_handle, 1);
	if ($nextchar=="\n"){
		$nextsql=$nextline;
		$j=strpos(" ".$nextline, "INSERT");
		if ($j!=1){
			echo $nextline."<br><br>";
			echo "ERROR ".$j;
			break;
		}
		$nextsql=substr($nextsql, 0,strlen($nextsql)-1);
		$db_obj->execute_statement($nextsql);
		echo "$i)&nbsp;<br>";
		//echo $nextsql."<br><br>";
		$nextline="";
		$i=$i+1;
	}else{
		$nextline.=$nextchar;
	}
}	
?>
