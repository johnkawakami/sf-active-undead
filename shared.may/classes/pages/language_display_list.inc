<?

class language_display_list extends Page
{

  function language_display_list()
  {
    return 1 ;
  }

  function execute()
  {
    $tr = new Translate();
	$language_obj = new \SFACTIVE\Language ;
        $lang_list = $language_obj->language_list();
	$language_hide = $tr->trans('hide');
	$language_show = $tr->trans('show');
	$reorder = $tr->trans('reorder');
	$tblhtml = "<form name=\"language_reorder\" action=\"/admin/language/language_reorder.php\">\n";
	while($row = array_pop($lang_list))
    {
		$tblhtml .= "<tr><td>".$row["id"]."</td>\n";
		$tblhtml .= "<td><a href=\"/admin/language/language_display_edit.php?id=".$row["id"]."\">".$row["name"]."</a></td>\n";
		if($row['build'] == 'y')
		{
		    $tblhtml .= "<td><small>(".$tr->trans('subsite').")</small></td>";
		}else{
		    $tblhtml .= "<td>&nbsp;</td>";
		}
		if($row['display'] == 't')
		{
			$tblhtml .= "<td><a href=\"/admin/language/language_hide.php?hide=".$row["id"]."\">".$language_hide."</a></td>\n";
		}else{
			$tblhtml .= "<td><a href=\"/admin/language/language_hide.php?show=".$row["id"]."\">".$language_show."</a></td>\n";
		}
		$tblhtml .= "<td><input name=\"order_num".$row["id"]."\" value=\"".$row["order_num"]."\"";
		$tblhtml .= "size=\"4\" maxlength=\"5\" /></td></tr>\n";
    }
	$tblhtml .= "<tr><td colspan=\"4\"><input type=\"submit\" name=\"reorder\" value=\"".$reorder."\"  /></form></td></tr>\n\n";
	$this->tkeys['table_middle'] = $tblhtml;
   return 1;
  }
}
?>
