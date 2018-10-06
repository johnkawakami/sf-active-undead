<?php

// Class for category_display_list page

class category_display_list extends Page
{

    function category_display_list()
    {
        return 1;
    }

    function execute()
    {

        // session_unregister("category_id");
        // session_unregister("category_name");

        $tr = new Translate();
        $category_obj = new \SFACTIVE\Category();

        $category_list = $category_obj->get_list();
// added category id ot the listing showing off in the list -- blicero
        while ($nextcategory = array_pop($category_list))
        {
            $tblhtml .= "<TR><TD><A href=\"".SF_ROOT_URL."/admin/category/category_display_edit.php?category_id=";
            $tblhtml .= $nextcategory['category_id'] . "\">";
            $tblhtml .= $nextcategory['category_id'] . "</a></td><td>";
	    	$tblhtml .= "<TD><A href=\"".SF_ROOT_URL."/admin/category/category_display_edit.php?category_id=";
            $tblhtml .= $nextcategory['category_id'] . "\">";
            $tblhtml .= $nextcategory['name'] . "</a>";
	    	$tblhtml .= "</td><td>";
		    // added catclass where needed :)))) --- blicero 20 may 2003
		    if (!is_null($nextcategory['catclass'])) $tblhtml .= "      ( ".$nextcategory['catclass']." ) </td><td>";

            if ($nextcategory['center'] == "t")
            {
                $tblhtml .= "<A href=\"".SF_ROOT_URL."/admin/feature/feature_display_list.php?category_id=";
                $tblhtml .= $nextcategory['category_id'] . "&category_name=";
                $tblhtml .= urlencode($nextcategory['name']) . "\">" . $tr->trans('story_list') . "</A>";
                $tblhtml .= "</td>";
            } else
            {
                $tblhtml .= $tr->trans('no_center_column') . "</TD>";
            }
			
			if($nextcategory['newswire'] !== 'n')
			{
				$tblhtml .= "<td>&nbsp;&nbsp;<a href=\"".SF_ROOT_URL."/refresh/refresh.php?cat=";
				if($nextcategory['category_id'] == $GLOBALS['config_defcategory'])
				{
					$tblhtml .= "main";
				}else{
					$tblhtml .= $nextcategory['category_id'] ;
				}
				$tblhtml .= "\">".$tr->trans('refresh_newswire')."</a></td>";
			}else{
				$tblhtml .= "<td>&nbsp;&nbsp;".$tr->trans('n_newswire')."&nbsp;</td>";
			}

            $tblhtml .= "<td><INPUT size=4 name=\"category_order";
            $tblhtml .= $nextcategory['category_id'];
            $tblhtml .= "\" value=\"";
            $tblhtml .= $nextcategory['order_num'];
            $tblhtml .= "\" /></TD></TR>";
        }

        $this->tkeys['table_middle'] = $tblhtml;
        return 1;

    }

}

?>
