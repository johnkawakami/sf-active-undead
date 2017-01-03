<?php

// Class for calendar_display_location_edit.inc

class calendar_display_location_edit extends Page {

    function calendar_display_location_edit() {
        // Class constructor, does nothing
        return 1;
    }

    function execute() {
		$tr = new Translate;
		$tr->create_translate_table('calendar');
		if (strlen($_GET['id']) > 0 || strlen($_POST['id']) > 0)
		{
			$is_edit = true;
			$this->tkeys['subtitle'] = $tr->trans('location_edit');
			$this->tkeys['local_form'] = "display_location_update.php";
			if (strlen($_POST['id']) > 0) $_GET['id'] = $_POST['id'];
		} else
		{
			$is_edit  = false;
			$this->tkeys['local_form'] = "display_location_add.php";
			$this->tkeys['subtitle'] = $tr->trans('location_add');
		}

		if ($is_edit === true)
		{
			$db = new DB ;
			$sql = "select name from location where location_id = ".$_GET['id'];
			$result_num = $db->query($sql);
			while($row = array_pop($result_num))
			{
				$this->tkeys['local_name'] = $row["name"];
			}
		}

		else {
			$this->tkeys['local_name'] = '';
		}
		$this->tkeys['local_id'] = $_GET['id'];
		return 1;
	}

}

?>
