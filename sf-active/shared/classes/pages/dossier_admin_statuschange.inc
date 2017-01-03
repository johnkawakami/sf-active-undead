<?

class dossier_admin_statuschange extends Page
{

	function dossier_admin_statuschange()
	{
		return 1;
	}

	function execute()
	{
		$db = new DB();
		$display = false;
		//print_r($_POST);
		foreach($_POST as $key => $value)
		{
			if($key !== "submit" && $value !== '')
			{
				$dossier_id = substr($key, 6, strlen($key - 6));
				$query = "update webcast w, webcast_dossier d set w.display = '".$value."' where w.id = d.webcast_id and d.id_dossier = '";
				$query .= $dossier_id."'";
				$result = $db->execute_statement($query);
			}
		}

		if($result == 0)
		{
			header('Location: index.php');
		}


	}

}
?>
