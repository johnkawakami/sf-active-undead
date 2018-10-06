<?php

// class used for /syn/index.php (builds the page & is used to cache local/cache/syndication_index.inc)

class Syndication_index extends Page
{
    function syndication_index()
    {
        return 1;
    }

    function execute()
    {
                $tr = new Translate ;

                $cache_file_name = SF_CACHE_PATH . '/syndication_index.inc';
        $file_rows = file($cache_file_name);
        $tblhtml = '';

        while (list($id,$text) = each($file_rows))
        {
            $tblhtml .= $text;
        }

                $tr->create_translation_table['syndication_index'];
                $this->tkeys['syndication_page']=$GLOBALS['dict']['syndication_page'];
                $this->tkeys['all_features']=$GLOBALS['dict']['all_features'];
                $this->tkeys['site_crumb']=$GLOBALS['site_crumb'];
                $this->tkeys['features']=$GLOBALS['dict']['features'];
                $this->tkeys['newswire']=$GLOBALS['dict']['newswire'];
				$this->tkeys['art_feat']=$GLOBALS['dict']['art_feat'];
				$this->tkeys['calendar']=$GLOBALS['dict']['calendar'];
				$this->tkeys['SYNDICATION_INDEX'] = $tblhtml;

                return 1 ;

    }

    function cache_syndication_list()
    {
                // This function selects all categories. For each category it will display links to the rss feeds of the
                // features (if category is set to have features) and to rss & xml feeds for the newswire (if the category
                // is set to have a newswire.
                // for the default cateogry it will use /newswire.rss & .xml (for the newswire) and for the features both
                // the production long & simple rdf and the general features.rdf long.rdf.
                // someone with a nicer method for the default category ?.
                $db_obj = new \SFACTIVE\DB;

				// let's only show show categories that are on the publish form.
				$newsfeed = $GLOBALS["syndication_page"] ;
				while ($char < strlen($newsfeed))
        		{
					$display_query .= "catclass='$newsfeed[$char]' or "; $char++;
				}
				$display_query = substr($display_query,0,strlen($display_query)-3);


                $query = "select name, shortname,  newswire, center, description, category_id from category where $display_query order by order_num desc " ;
				//echo $query ;
                $resultset = $db_obj->query($query) ;
				$syndication_index = '<tr><td><h3>TPL_ART_FEAT</h3></td></tr>';
                while($row = array_pop($resultset))
                {
                  if(($row["center"] == "f") && ($row["newswire"] == "n")) { ; }
                  else
                  {
					$syndication_index .= '<tr><td><b>'.$row["name"].'</b></td></tr>' ;
					$syndication_index .= '<tr><td class="bgsearchgrey">'.$row["description"].'<br />' ;
					if($row["center"] == "t")
					{
						$syndication_index .= '<a href="'.SF_ROOT_URL.'/syn/'.$row["shortname"]. '_'. $GLOBALS['feature_file'].'">TPL_FEATURES</a> (simple ) - <a href="/syn/'.$row["shortname"].'_'.$GLOBALS['feature_file_long'].'">TPL_FEATURES</a>  (with content) - ' ;
                                if($row["category_id"] == $GLOBALS["config_defcategory"])
                                {
                                        $syndication_index .= '<br /><a href="'.SF_ROOT_URL.'/syn/'.$GLOBALS['feature_file'].'">TPL_ALL_FEATURES</a>  (simple) - <a href="/syn/'.$GLOBALS['feature_file_long'].'">TPL_ALL_FEATURES</a>  (with content)<br />' ;
                                }
                        }
                        if($row["newswire"] !== "n")
                        {
                                if($row["category_id"] == $GLOBALS["config_defcategory"])
                                {
                                        $syndication_index .= '<a href="'.SF_ROOT_URL.'/newswire.rss">TPL_NEWSWIRE</a> (rss) - <a href="'.SF_ROOT_URL.'/newswire.xml">TPL_NEWSWIRE</a> (xml)</td></tr><tr><td>&nbsp;</td></tr>' ;
                                }
                                else
                                {
                                        $syndication_index .= '<a href="'.SF_ROOT_URL.'/syn/'.$row["shortname"].'.rdf">TPL_NEWSWIRE (rdf)</a> - <a href="'.SF_ROOT_URL.'/syn/'.$row["shortname"].'.xml">TPL_NEWSWIRE (xml)</a></td></tr><tr><td>&nbsp;</td></tr>' ;
                                }
                        }
                        else
                        {                    
							$syndication_index .= '</td></tr><tr><td>&nbsp;</td></tr>' ;
                        }
                  }
                }

		if($GLOBALS['syndicate_calendar'] == "1")
		{
		  $result = $this->get_calendar_list();
		  $syndication_index .= "<tr><td><h3><a name=\"cal\" id=\"cal\"></a>TPL_CALENDAR</h3></td></tr>";
		  $syndication_index .= "<tr><td class=\"bgsearchgrey\"><a href=\"".SF_ROOT_URL."/syn/" ;
		  $syndication_index .= $GLOBALS['calendar_file']."\">TPL_CALENDAR</a><br />";
		  while($row = array_pop($result))
		  {
		  	$syndication_index .= "<a href=\"".SF_ROOT_URL."/syn/".strtolower(str_replace(" ", "_", $row["name"]))."_".$GLOBALS['calendar_file']."\">" ;
			$syndication_index .= $row["name"]."</a> - ";
		  }
		  $syndication_index = substr($syndication_index, 0, strlen($syndication_index)-2);
		  $syndication_index .= "</td></tr>";
		}


		$fffp = fopen(SF_CACHE_PATH."/syndication_index.inc", "w");
        fwrite($fffp, $syndication_index, strlen($syndication_index));
        fclose($fffp);
        }

		function get_calendar_list()
		{
			$db = new \SFACTIVE\DB;
			$sql = "select name from location order by name asc";
			$result = $db->query($sql);
			return $result;
		}

}
?>
