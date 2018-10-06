<?php

// Class for category_display_edit page

class category_display_edit extends Page
{

    function category_display_edit()
    {
        return 1;
    }

    function execute()
    {

        $tr = new Translate;

        $news_options = array(
                            "n" => $tr->trans('n_newswire'),
                            "s" => $tr->trans('s_newswire'),
                            'f' => $tr->trans('f_newswire'),
                            "a" => $tr->trans('a_newswire')
                            );

        $cent_options = array(
                            "f" => $tr->trans('no_centercolumn'),
                            "t" => $tr->trans('standard_centercolumn')
                            );
	// add catclass array to choose the type of category
	$catclass_options = array (
			    "m" => $tr->trans('m_category'),
			    "t" => $tr->trans('t_category'),
			    "l" => $tr->trans('l_category'),
			    "h" => $tr->trans('h_category'),
			    "i" => $tr->trans('i_category'),
			    "e" => $tr->trans('e_category'),
			    "p" => $tr->trans('p_category'),
			    "o" => $tr->trans('o_category'),
			    );
	// include the array crating the templates from include dir so that everyone can customize it :) --- blicero	
	include(SF_INCLUDE_PATH."/select_templates.inc");

        $this->tkeys['local_category_name'] = '';
        $this->tkeys['local_category_id'] = 0;
        $this->tkeys['local_ordernum'] = '';
// set defautl template name to feature_list since otherwise category don't work -- blicero 20 may 2003
        $this->tkeys['local_template_name'] = 'feature_list';
        $this->tkeys['local_default_feature_template_name'] = '';
        $this->tkeys['local_category_shortname'] = '';
        $this->tkeys['local_summarylength'] = '';
        $this->tkeys['local_parentid'] = 1;
        $this->tkeys['local_newswire'] = '';
        $this->tkeys['local_center'] = '';
        $this->tkeys['local_description'] = '';

        if (strlen($_GET['category_id']) > 0 || strlen($_POST['category_id']) > 0)
        {
            $is_edit = true;
            $this->tkeys['subtitle'] = $tr->trans('category_edit');
            $this->tkeys['local_form'] = "category_update.php";
            if (strlen($_POST['category_id']) > 0) $_GET['category_id'] = $_POST['category_id'];
        } else
        {
            $is_edit  = false;
            $this->tkeys['local_form'] = "category_add.php";
            $this->tkeys['subtitle'] = $tr->trans('category_add');
        }

        if ($is_edit === true)
        {
            $category_obj = new \SFACTIVE\Category;
            $category = $category_obj->get_category_fields($_GET['category_id']);
	// added by blicero to have category class
            $this->tkeys['local_catclass'] = $category['catclass'];
            $this->tkeys['local_category_name'] = $category['name'];
            $this->tkeys['local_category_id'] = $category['category_id'];
            $this->tkeys['local_ordernum'] = $category['order_num'];
            $this->tkeys['local_template_name'] = $category['template_name'];
            $this->tkeys['local_default_feature_template_name'] = $category['default_feature_template_name'];
            $this->tkeys['local_category_shortname'] = $category['shortname'];
            $this->tkeys['local_summarylength'] = $category['summarylength'];
            $this->tkeys['local_parentid'] = 1;
            $this->tkeys['local_newswire'] = $category['newswire'];
            $this->tkeys['local_center'] = $category['center'];
            $this->tkeys['local_description'] = $category['description'];
        }

        $this->tkeys['local_select_template'] = $this->make_select_form("default_feature_template_name", $temp_options, $category['default_feature_template_name']);

        $this->tkeys['local_select_newswire'] = $this->make_select_form("newswire", $news_options, $category['newswire']);
        $this->tkeys['local_select_centercolumn'] = $this->make_select_form("center", $cent_options, $category['center']);
	// added for category class -- blicero
        $this->tkeys['local_select_catclass'] = $this->make_select_form("catclass", $catclass_options, $category['catclass']);

        return 1;
    }
}

?>
