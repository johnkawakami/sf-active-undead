<?php

// Class for feature_add page

class feature_copy extends Page
{

    function feature_copy()
    {        
	if ($_POST["category"]!="0"){
		include_once(SF_SHARED_PATH . "/classes/category_class.inc");
            	$_SESSION['category_id'] = $_POST['category'];
		$cat_obj=new Category;
		$cat_dict=$cat_obj->get_category_fields($_POST['category']);
		$category_name=$cat_dict["name"];
            	$_SESSION['category_name'] = $category_name;
		include("feature_display_edit.php");
		return 1;
	}else{
		include("feature_display_copy.php");
		return 1;
	}
	
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>