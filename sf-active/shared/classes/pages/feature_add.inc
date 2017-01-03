<?php

// Class for feature_add page

class feature_add extends Page
{

    function feature_add()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

        include(SF_SHARED_PATH . '/classes/feature_class.inc');
	if($_POST['category_id']) { $_SESSION['category_id'] = $_POST['category_id'] ; }

        $feature_obj=new Feature;

        $create_date = $_POST['creation_date_year'] . "-" . $_POST['creation_date_month'] . "/" . $_POST['creation_date_day'];
        $create_date .= " 00:00:00";

        $featurefields=array(
                            title1 =>           $_POST['title1'],
                            title2 =>           $_POST['title2'],
                            display_date =>     $_POST['display_date'],
                            creation_date =>    $create_date,
                            summary =>          $_POST['summary'],
                            order_num =>        $_POST['order_num'],
                            status =>           $_POST['status'],
                            tag =>              $_POST['tag'],
                            image =>            $_POST['image'],
                            image_link =>       $_POST['image_link'],
                            category_id =>      $_SESSION['category_id'],
                            template_name =>    $_POST['template_name'],
			    language_id =>	$_POST['language_id'],
                            );

        $error_num = $feature_obj->add($featurefields);
        header("Location: feature_display_list.php");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
