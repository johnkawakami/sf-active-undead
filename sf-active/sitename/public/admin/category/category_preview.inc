<?php
//get category name,heaeder and footer for display and file generation purposes

$query="select name,header, footer from category where category_id=";
$query=$query.$category_id;
$category_rs=$db_obj->execute_getresultset($query);
$category_row=array_pop($category_rs);
$category_name=$category_row[name];
$header=$category_row[header];
$footer=$category_row[footer];
$result_html=$header;
$result_html=$result_html."<table width=\"100%\"><tr ><td >";
$result_html=$result_html."<TABLE width=\"100%\" border=0 cellpadding=\"2\">";	

#get feature list
$query="select * from feature where category_id=";
$query=$query.$category_id;
$query=$query." and is_current_version=1 and status='c' order by order_num desc";
$feature_list=$db_obj->execute_getresultset($query);

// set up fasttemplate
$tpl = new FastTemplate(SF_TEMPLATE_PATH);

$tpl->define(array(blurb => "blurb.tpl"));
while ($nextfeature=array_pop($feature_list)){	
$defaults = (array(    TITLE1     => "$nextfeature[title1]",		
CRDATE     => "$nextfeature[display_date]",
TITLE2     => "$nextfeature[title2]",
IMAGE	   => "$nextfeature[image]",
TAG		   => "$nextfeature[tag]",
SUMMARY    => "$nextfeature[summary]"));
$tpl->assign($defaults);
$tpl->parse(CONTENT, "blurb");
$result_html .= $tpl->fetch("CONTENT");
}

$result_html=$result_html."</TABLE></TD></TR></TABLE>".$footer;

?>
