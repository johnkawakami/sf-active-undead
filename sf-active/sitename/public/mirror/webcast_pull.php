<?
include("shared/global.cfg");

if (strlen($is_mirror)<1){
echo "WARNING THIS CODE SHOULD ONLY BE RUN ON MIRRORED VERSIONS OF A SITE";
exit;
}

include_once(SF_SHARED_PATH."/classes/newswire_class.inc");

$MIRRORED_SITE_URL="http://sf.indymedia.org";
//$MIRRORED_SITE_URL="http://127.0.0.1";

$webcast_url=$MIRRORED_SITE_URL."/mirror/webcast_dump.php";

$db_obj= new DB;

$min_webcast_id=0;
$min_webcast_comment_id=0;
$min_webcast_cat_id=0;

$newswire_obj= new Newswire;
$min_webcast_id=$newswire_obj->get_max_post_id();
$min_webcast_comment_id=$newswire_obj->get_max_comment_id();
$min_webcast_cat_id=$newswire_obj->get_max_cat_id();

$webcast_url.="?min_webcast_id=$min_webcast_id&min_webcast_comment_id=$min_webcast_comment_id&min_webcast_cat_id=$min_webcast_cat_id&num_rows=500";
echo $webcast_url;
//exit;
$file_handle=fopen($webcast_url,"r");
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
