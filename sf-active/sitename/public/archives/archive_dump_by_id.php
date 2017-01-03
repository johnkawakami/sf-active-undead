<?
include "shared/global.cfg";
include("$shared_classes_path/category_class.inc");
include("$shared_classes_path/feature_class.inc");


if (!isset($feature_id)) {
header ("Location: ".$webroot_url."/archives/index.php");
}

$db_obj= new DB;
$category_obj= new Category;
$feature_obj= new Feature;
$template_obj= new FastTemplate($local_templates_path);

$body=$category_obj->render_single_feature_archive($feature_id);
echo $body;
?>


