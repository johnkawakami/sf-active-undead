<!-- start center column -->

	<td width="100%" valign="top">
	<!-- "the very first font tag" -ian -->
	<font size="-1">
<?php if (strlen($preview)>0){
		include("shared/db_class.inc");
		$db_obj = new DB;
		include("shared/const.inc");
		include_once($rootdir."/shared/category_class.inc");
		$category_obj= new Category;
		include($rootdir."/shared/feature_class.inc");
		$feature_obj= new Feature;
		include($rootdir."/shared/template_class.inc");
		$template_obj= new FastTemplate($rootdir."/shared/templates");
		echo $category_obj->render_feature_list($preview);
}else{
?>

<?php include("local/webcast/cache/center_column_production.html"); ?>

<?php
}
?>

</font><font color="#c0c0c0" face="arial,helvetica,sans-serif" size="2">

<!-- archive --><p><strong><font size="3"><a href="/archives/">Archived highlights</a><a href="/archives/">>></a></font></strong><!-- /archive -->

<!-- date script by mark B. -->
<!-- DO NOT CHANGE THIS PLEASE  -->
<font face="verdana,arial,helvetica" size="-2"><br><br>Last updated: <?php
echo date("D M j H:i:s T Y",filemtime("$rootdir/local/webcast/cache/center_column_production.html"));
?><?php include ('shared/front-imc-footer.inc'); ?> <br></font></font></td>


<!-- /end center column -->
