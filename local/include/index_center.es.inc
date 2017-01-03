<?php

if (strlen($_GET['preview']) > 0)
{
    include_once(SF_SHARED_PATH . '/classes/category_class.inc');
    $category_obj= new Category;

    include(SF_SHARED_PATH . '/classes/feature_class.inc');
    $feature_obj= new Feature;

    $template_obj= new FastTemplate(SF_TEMPLATE_PATH);

    echo $category_obj->render_feature_list($_GET['preview']);
} else
{
    include(SF_CACHE_PATH . '/center_column_production.html');
}

?>

<h3><a href="/archives/archive_display_by_date.php">[ &nbsp; Older Stories &nbsp; ]</a></h3>

<!-- 
Donate Button Exmaple
<p>
<a href="http://www.guidestar.org/partners/networkforgood/donate.jsp?ein=94-2563400">
<img src="/images/donate.gif" width="244" height="26" border="0" alt="Donate now through Network for Good." />
</a>
</p>
-->
