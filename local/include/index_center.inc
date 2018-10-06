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
        if ($GLOBALS['isWap']) {
                include(SF_INSTALL_PATH . '/local/include/waplayout.inc'); 
        } else if ($GLOBALS['isMobile']) {
                include(SF_INSTALL_PATH . '/local/include/mobilelayout.inc'); 
        } else if (strlen($feature_name)>0){
		include(SF_CACHE_PATH. '/center_column_'.$feature_name.'.html');
	}else{
		include(SF_CACHE_PATH . '/center_column_production.html');
	}
}

  if (strlen($category_id)>0 and !$GLOBALS['isMobile']) {
?>
<h3><a href="/archives/archive_display_by_date.php?category_id=<?=$category_id?>">[   Older Stories   ]</a></h3>
<?php
  } else if (!$GLOBALS['isMobile']) {
?>
<h3><a href="/archives/archive_display_by_date.php">[   Older Stories   ]</a></h3>
<?php
  }
?>
