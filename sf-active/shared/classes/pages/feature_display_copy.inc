<?php

// Class for feature_display_edit page

class feature_display_copy extends Page
{

    function feature_display_copy()
    {
	return 1;
    }

    function execute()
    {
		include_once(SF_SHARED_PATH . '/classes/category_class.inc');
		$this->tkeys['local_title1']=$_POST['title1'];
		$this->tkeys['local_title2']=$_POST['title2'];	
                $this->tkeys['local_display_date']=$_POST['display_date'];
		$summary=str_replace("\"","&quot;",$_POST['summary']);	
                $this->tkeys['local_summary']=stripslashes($summary);
                $this->tkeys['local_order_num']=$_POST['order_num'];		
                $this->tkeys['local_tag']=$_POST['tag'];	
                $this->tkeys['local_img_link']=$_POST['image_link'];	
                $this->tkeys['local_template']=$_POST['template_name'];	
                $this->tkeys['local_image']=$_POST['image'];
		$this->tkeys['local_language_id']=$_POST['language_id'];
        	$cat_opt = new Category;
		/* replace this by a static variant.
        	$cat_options = $cat_opt->get_feature_assoc_list(); */
		include_once(SF_CACHE_PATH.'/feature_options.inc');

        	$this->tkeys['category_select'] = $this->make_select_form("category", $cat_options, 		$_POST['category']);  
		$this->tkeys['local_form_action']='feature_copy.php';	
		$this->tkeys['local_history_link']="";
        return 1;
    }
}

?>
