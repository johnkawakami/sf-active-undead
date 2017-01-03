<?
include("shared/global.cfg");
include_once(SF_SHARED_PATH."/classes/backup_class.inc");
$backup= new Backup;

$min_webcast_id=0;
$min_webcast_comment_id=0;
$min_webcast_cat_id=0;
$num_rows=100;

if (strlen($_GET["min_webcast_id"])>0){
$min_webcast_id=$_GET["min_webcast_id"];
//prevent hacks
if (strlen($min_webcast_id)>10 || !is_numeric($min_webcast_id)){
echo "HACK ALERT!!\n\n";
exit;
}
}

if (strlen($_GET["min_webcast_comment_id"])>0){
$min_webcast_comment_id=$_GET["min_webcast_comment_id"];
//prevent hacks
if (strlen($min_webcast_comment_id)>10 || !is_numeric($min_webcast_comment_id)){
echo "HACK ALERT!!\n\n";
exit;
}
}

if (strlen($_GET["min_webcast_cat_id"])>0){
$min_webcast_cat_id=$_GET["min_webcast_cat_id"];
//prevent hacks
if (strlen($min_webcast_cat_id)>10 || !is_numeric($min_webcast_cat_id)){
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
$sql1="Select * from webcast where parent_id=0 and (display='l' or display='g') and id>$min_webcast_id order by id limit $num_rows";
echo $backup->get_table_content("webcast", $sql1);
$sql2="Select w1.* from webcast w1, webcast w2 where w1.parent_id=w2.id and w1.display!='f' and(w2.display='l' or w2.display='g') and w1.id>$min_webcast_comment_id order by w1.id limit $num_rows";
//echo $sql2;
echo $backup->get_table_content("webcast", $sql2);
$sql3="Select cl.* from catlink cl, webcast wc where cl.id=wc.id and (wc.display='l' or wc.display='g') and wc.id>$min_webcast_cat_id order by wc.id limit $num_rows";
//echo $sql3;
echo $backup->get_table_content("catlink", $sql3);

?>