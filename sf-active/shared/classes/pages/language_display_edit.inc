<?php

// Class for language_display_edit page

class language_display_edit extends Page
{

    function language_display_edit()
    {
        return 1;
    }

    function execute()
    {
	$tr = new Translate;
	$tr->create_translate_table('language');
	if (strlen($_GET['id']) > 0 || strlen($_POST['id']) > 0)
        {
            $is_edit = true;
            $this->tkeys['subtitle'] = $tr->trans('language_edit');
            $this->tkeys['local_form'] = "language_update.php";
            if (strlen($_POST['id']) > 0) $_GET['id'] = $_POST['id'];
        } else
        {
            $is_edit  = false;
            $this->tkeys['local_form'] = "language_add.php";
            $this->tkeys['subtitle'] = $tr->trans('language_add');
        }
       $build_options = array(
            'y' => $tr->trans('yes'),
            'n' => $tr->trans('no'));

	$display_options = array(
	    't' => $tr->trans('show'),
	    'f' => $tr->trans('hide'));

	if ($is_edit === true)
	{
	$lang = new language;
	$result_num = $lang->language_edit($_GET['id']);
	while($row = array_pop($result_num))
	{
		$this->tkeys['local_name'] = $row["name"];
		$this->tkeys['local_language_code'] = $row["language_code"];
		$this->tkeys['local_order_num'] = $row["order_num"];
		$this->tkeys['local_display'] = $this->make_select_form('display', $display_options, $row['display']);
		$this->tkeys['checkbox_build'] = $this->make_select_form('build', $build_options, $row['build']);
	}
	}
	else {
	$this->tkeys['local_name'] = '';
	$this->tkeys['local_language_code'] = '';
	$this->tkeys['local_display'] = $this->make_select_form('display', $display_options, 't');
	$this->tkeys['local_order_num'] = '';
	$this->tkeys['local_select_hide'] = "";
	$this->tkeys['checkbox_build'] = $this->make_select_form('build', $build_options, "n");
	}
	$this->tkeys['id'] = $_GET['id'];
    $this->tkeys['name'] = $tr->trans('language');
	$this->tkeys['subtitle'] = $tr->trans('language_edit');
        return 1;
	}
}

?>
