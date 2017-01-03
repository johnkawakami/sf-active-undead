<?
$min_feature_id=0;
$min_category_id=0;
$num_rows=1000;

if (strlen($_GET["min_feature_id"])>0){
$min_feature_id=$_GET["min_feature_id"];
//prevent hacks
if (strlen($min_feature_id)>10 || !is_numeric($min_feature_id)){
echo "HACK ALERT!!\n\n";
exit;
}
}

if (strlen($_GET["min_category_id"])>0){
$min_category_id=$_GET["min_category_id"];
//prevent hacks
if (strlen($min_category_id)>10 || !is_numeric($min_category_id)){
echo "HACK ALERT!!\n\n";
exit;
}
}

if (strlen($_GET["num_rows"])>0){
$num_rows=$_GET["num_rows"];
//prevent hacks
if (strlen($num_rows)>6 || !is_numeric($num_rows)){
echo "HACK ALERT!!\n\n";
exit;
}
}

include("shared/global.cfg");
include_once(SF_SHARED_PATH."/classes/backup_class.inc");
$backup= new Backup;

$category_sql="Select * from category";
if ($min_category_id>0){
	$category_sql.=" where category_id>".$min_category_id;	
}
$category_sql.=" order by category_id";
echo $backup->get_table_content("category", $category_sql);
//echo $category_sql;

$feature_sql="Select * from feature where is_current_version=1";
if ($min_feature_id>0){
	$feature_sql.=" and feature_version_id>".$min_feature_id;	
}
$feature_sql.=" order by feature_version_id limit $num_rows";
echo $backup->get_table_content("feature", $feature_sql);

?>
