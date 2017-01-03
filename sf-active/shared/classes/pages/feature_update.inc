<?php

// Class for feature_update page

class feature_update extends Page
{

    function feature_update()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

	if (strlen($_POST['copy'])>0){
		include("feature_display_copy.php");
		//header("Location: feature_display_copy.php");
		exit;
	}
        global $db_obj;

        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');

        $feature_obj = new Feature;

        $create_date = $_POST['creation_date_year'] . "-" . $_POST['creation_date_month'] . "/" . $_POST['creation_date_day'];
        $create_date .= " 00:00:00";

        $featurefields = array(
                            feature_version_id =>   $_POST['feature_id'],
                            feature_id=>            $_POST['feature_id'],
                            title1=>                $_POST['title1'],
                            title2=>                $_POST['title2'],	
                            display_date=>          $_POST['display_date'],	
                            creation_date=>         $create_date,
                            summary=>               $_POST['summary'],
                            order_num=>             $_POST['order_num'],	
                            status=>                $_POST['status'],	
                            tag=>                   $_POST['tag'],	
                            image_link=>            $_POST['image_link'],	
                            template_name=>         $_POST['template_name'],	
                            image=>                 $_POST['image'],	
                            category_id=>           $_SESSION['category_id'],
			    language_id =>	    $_POST['language_id'],
                            );

        $error_num = $feature_obj->update($featurefields);
        header("Location: feature_display_list.php?status=" . $_POST['status']);
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
