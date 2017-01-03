<?php

// Class for category_display_pushtoproduction_confirmation page

class category_display_pushtoproduction_confirmation extends Page
{

    function category_display_pushtoproduction_confirmation()
    {
        return 1;
    }

    function execute()
    {
        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');
	
        $this->tkeys['local_category_name'] = $_SESSION['category_name'];
        $Category=new Category;
//        $Category->make_features_rdf();
                $Category->make_features_rdf("", "");
                $Category->make_features_rdf("", $_SESSION['category_name']);
                $Category->make_features_long_rdf("summary", "");
                $Category->make_features_long_rdf("summary", $_SESSION['category_name']);
	
	// code to generate single feature archive pages to be displayed by link in the rdf :)
        $feature_obj=new Feature();
	$feature_list=$feature_obj->get_list($_SESSION['category_id'], "c");
        
	while ($nextfeature=array_pop($feature_list))
        {
            $msg.=$Category->render_single_feature_archive($nextfeature[feature_id]);
        }
	$this->tkeys['local_writing_msg']=$msg;
		

	
        return 1;
    }
}

?>
