<?

class calendar_update extends page
{

	function calendar_update()
	{
  		return 1;
	}

	function execute()
	{
		include_once(SF_CLASS_PATH."/pages/calendar_edit.inc");
		$cal = new calendar_edit;
		$cal->execute();
	}

}
?>
