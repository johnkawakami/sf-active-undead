<?php
namespace SFACTIVE;

class Newswire extends \Cache
{
    // This is the replacement for summary_list.inc, which generates newswire

    function render_mime_image($mimetype)
    {
        // Generates HTML for the small mimetype icon
        $icon="";
        if (preg_match("/audio/",$mimetype)) $icon="imc_audio.gif";
        if (preg_match("/image/",$mimetype) OR preg_match("/shockwave/",$mimetype)) $icon="imc_photo.gif";
        if (preg_match("/video/",$mimetype) OR preg_match("/smil/",$mimetype)) $icon="imc_video.gif";
        if (preg_match("/pdf/",$mimetype)) $icon="imc_pdf.gif";
        if (!$icon) $icon="imc_article.gif";

        $iconstring = "<img src=\"".SF_ROOT_URL."/im/$icon\" border=\"0\" alt=\"\" />";

        return $iconstring;
    }

    function get_summary_list($length, $post_status, $cat)
    {
        // Returns a resultset of articles for a newswire summary list
	    if(!$this->max_id) $this->get_max_id() ;
        $db_obj = new \SFACTIVE\DB();
        global $time_diff;
	    $op="+";
		if ($time_diff<0){
			$ltimediff=$time_diff*-1;
			$op="-";
		}
		else{
		    $ltimediff=$time_diff;
		}
		// here we set the minimum date articles should be published in. this does speed up your query
		/*
        if($GLOBALS['site_size'] == "big")
		{
			$min_date = mktime(0,0,0,date('m'), date('d') - 2, date('Y'));
		}else{
			$min_date = mktime(0,0,0,date('m') -1, date('d'), date('Y'));
		}
		$date_limit_query = "and w.created > '".date('Y-m-d', $min_date)."' "; 
        */


		// this should be better then date_limit, more flexible and uses primary_key on webcast
		if($cat)
		{
		    if(!$GLOBALS['nw_c_range']) $GLOBALS['nw_c_range'] = 3000;
		    $limit_ids = $this->max_id - $GLOBALS['nw_c_range'];
		}else{
		    if(!$GLOBALS['nw_range']) $GLOBALS['nw_range'] = 500;
		    $limit_ids = $this->max_id - $GLOBALS['nw_range'];
		}		    
	    if($GLOBALS['isWap']) $limit_ids = $this->max_id - 1000;
		$id_limit_query = " and w.id > ".$limit_ids." ";
		

        $query = "SELECT w.id as id, w.linked_file as linked_file, 
                        w.author as author, w.numcomment as numcomment, w.heading as heading, w.mime_type as mime_type,
                        date_format(w.created, '%Y') as created_year,
                        date_format((w.created $op interval $ltimediff second), '%b %d %l:%i%p') as created,
                        date_format(w.created, '%m') as created_month,
                        date_add(w.created, interval 48 hour) as plus_twelve, now() as now, w.language_id as language_id
                        FROM webcast_latest w";
        if ($cat > 0) $query .= ",catlink l";
        if ($post_status=="lg") {
            $query .= " where (w.display='l' or w.display='g') and ";
        } else {
            $query .= " where w.display='" . $post_status . "' and ";
        }

        if ($cat > 0) $query .= "l.id = w.id and l.catid=$cat and ";
        $query .= "w.parent_id=0 ".$id_limit_query." ORDER BY w.id DESC limit $length";
		
		$fh = fopen("/tmp/sf-active-get-summary-list","w+");
		fwrite($fh, $query."\n");
		fclose($fh);

        $resultset = $db_obj->query($query);
        return $resultset;
    }

    function render_summary_list($resultset)
    {
	$lang_obj = new \SFACTIVE\Language ;
        // Returns HTML from a template given a summary list resultset
        $template_obj = new \FastTemplate(SF_TEMPLATE_PATH);

        while ($row = array_pop($resultset))
        {
            // Set up template variables
            $link = $this->render_yearmonth_link($row['created_year'], $row['created_month']);
            $articlelink = SF_NEWS_URL."/" . $link . $row['id'] . ".php";

            $iconhtml = $this->render_mime_image($row['mime_type']);
            $row['created'] = $this->render_protest_date($row['created']);
            $row['heading'] = $this->render_entities($row['heading']);

            $counter = $row['numcomment'];
            $iter = 0;
            while ($counter > 0 and $iter < 5) {
                $row['n_c_symbol'] .= '&#8226;';
                $counter = $counter - 5;
                $iter = $iter + 1;
            }

	    $lang_code = $lang_obj->get_language_code($row['language_id']);
            // Set up template array
            $defaults=(array(   'I_CON'           => $iconhtml,
                                'A_RTICLELINK'    => $articlelink,
                                'H_EADING'        => $row['heading'],
                                'A_UTHOR'         => $row['author'],
                                'N_COMMENT'       => $row['numcomment'],
                                'N_C_SYMBOL'      => $row['n_c_symbol'],
                                'D_ATE'           => $row['created'],
				'L_ANGUAGE'	=> $lang_code
                                ));

            // Process the template
            $result_html .= $template_obj->render_template($defaults, "summary_row.tpl");
        }

        return $result_html;
    }

    function render_summary($categorydata)
    {
        // Returns HTML of a complete summary list 
        $template_obj = new \FastTemplate(SF_TEMPLATE_PATH);

        $link = "";
		$tr = new \Translate ;
		$tr->create_translate_table('newswire');

        $summarylength = $categorydata['summarylength'];
        $summaryfile = SF_CACHE_PATH . "/" . $categorydata['shortname']."_summaries.html";
        $summaryname = $categorydata['name'];
        $summarytype = $categorydata['newswire'];
        $cat = $categorydata['category_id'];

	// rebuild the rss feed -jk's hack
	$this->make_newswire_rss($cat);

        if ($GLOBALS['config_defcategory'] == $cat) $summaryfile = SF_CACHE_PATH . "/summaries.html";
        if ($GLOBALS['config_defcategory'] == $cat) $cat = 0;

        // Evaluate Standard vs Local/Global/Open Newswire style
        switch ($summarytype)
        {
            case "n":
                $Result .= $tr->trans('no_newswire');
                break;

            case "s":
                // Get the summary list, render it and cache it
                $resultset = $this->get_summary_list($summarylength, "t", $cat);
                $resulthtml = $this->render_summary_list($resultset);
                $this->cache_file($resulthtml, $summaryfile);
                $Result .= $tr->trans('newswire_refreshed_for').$summaryname."<br />";
                break;

            case "f":
                // Filtered Newswire -- l & g
				// johnk - no longer l & g, just l
				// it's faster
                $localglobal=$this->get_summary_list($summarylength, "l", $cat);
                $localglobal_wire = $this->render_summary_list($localglobal);
                if ($cat) $link = "&amp;category=" . $cat;
                $defaults=(array(   'LG_NEWSWIRE'  => "$localglobal_wire",
                                    'S_LINK'          => "$link" ));
                $resulthtml .= $template_obj->render_template($defaults, "summary_newswirelg.tpl");
                $this->cache_file($resulthtml, $summaryfile);
                $Result .= $tr->trans('refreshed_all').$summaryname."<br />";
                break;

            case "a":
                // Get the summary list, render and cache it for all 3 newswires
                $local = $this->get_summary_list($summarylength, "l", $cat);
                $local_wire = $this->render_summary_list($local);

                $global = $this->get_summary_list($summarylength, "g", $cat);
                $global_wire = $this->render_summary_list($global);

                $open = $this->get_summary_list($summarylength, "t", $cat);
                $open_wire = $this->render_summary_list($open);

                if ($cat) $link = "&amp;category=" . $cat;

                $defaults=(array(   'L_OCALNEWSWIRE'  => "$local_wire",
                                    'S_LINK'          => "$link",
                                    'G_LOBALNEWSWIRE' => "$global_wire",
                                    'O_PENNEWSWIRE'   => "$open_wire"
                                    ));

                $resulthtml .= $template_obj->render_template($defaults, "summary_newswire.tpl");
                $this->cache_file($resulthtml, $summaryfile);
                $Result .= $tr->trans('refreshed_all').$summaryname."<br />";
                break;

            default:
                $Result .= $tr->trans('unable_to_determine'); 
        }
        return $Result;
    }

    function get_syndicated_articles($cat = 0)
    {
        // function to get articles for syndication. $newsfeed could be "l" or "lgtf"
	if($GLOBALS['config_defcategory'] == $cat ) { $cat = 0 ; }

	if(!$this->max_id) $this->get_max_id();
	

        $newsfeed = $GLOBALS['newsfeed'];
        $news_length = $GLOBALS['news_length'];
        $db_obj = new \SFACTIVE\DB;
		$char = 0;
        while ($char < strlen($newsfeed))
        {
            $display_query .= "display='".substr($newsfeed, $char, 1)."' or "; $char++;
        }

	// we add an id limiter. this forces mysql to use the primary_key & to select
	// in a much smaller range. result: much faster queries.
	// if they're not defined, we set some quite sane defaults (should be easier
	// to upgrade running sites with it.
	if($cat)
	{
	    if(!$GLOBALS['nwf_c_range']) $GLOBALS['nwf_c_range'] = 3000 ;
	    $id_limiter = $this->max_id - $GLOBALS['nwf_c_range'] ;
	    $limit_query = " w.id > ".$id_limiter." and ";
	}
	else
	{
	    if(!$GLOBALS['nwf_range']) $GLOBALS['nwf_range'] = 3000 ;
	    $id_limiter = $this->max_id - $GLOBALS['nwf_range'] ;
	    $limit_query = " id > ".$id_limiter." and ";
	}

        $display_query = preg_replace("/or $/","",$display_query);
        $query = "select display,numcomment,id,linked_file,author,heading,mime_type,created, summary, artmime,article, language_id from webcast where $limit_query ($display_query) and parent_id=0 ORDER BY id DESC limit $news_length";

// corrected the following query to accept correctly the display_query :)))) -- blicero
        if ($cat > 0 && $GLOBALS['config_defcategory'] !== $cat)
        {
			$display_query = preg_replace('/display/','w.display',$display_query);
            $query = "SELECT w.display as display,
                             w.numcomment as numcomment,
                             w.id as id,
                             w.linked_file as linked_file,
                             w.author as author,
                             w.heading as heading,
                             w.mime_type as mime_type,
                             w.created as created,
                             w.summary as summary,
                             w.artmime as artmime,
                             w.article as article,
							 w.language_id as language_id
                      FROM webcast_latest w, catlink c
                      WHERE 
						$limit_query
                        parent_id = 0
						AND (".$display_query.")
                        AND w.id = c.id
                        AND c.catid = $cat
                      ORDER BY id
                      DESC limit $news_length";
            
        }
        $fh = fopen('/tmp/sf-active-catlist','w+');
        fwrite($fh,$query);
		fwrite($fh,"\n-------------------------------------\n");
        fclose($fh);
        $resultset = $db_obj->query($query);
        return $resultset;
    }

    function make_newswire_rss($cat = 0, $name = '')
    {
	$GLOBALS['print_rss'] = "" ;
	include_once(SF_CLASS_PATH.'/rss10.inc');
	    $lang_obj = new \SFACTIVE\Language ;
        $resultset = $this->get_syndicated_articles($cat);

        $rows = sizeof($resultset);

	if ($rows == 0) {
		echo "found no articles for category $cat<br>";
		return;
	}

        $xml_logo = $GLOBALS['xml_logo'];

        if (strlen($name) > 0)
        {
            $xml_file = SF_WEB_PATH . '/syn/' . $name . '.xml';
            $rdf_file = SF_WEB_PATH . '/syn/' . $name . '.rdf';
            $rdf_url = SF_ROOT_URL . '/syn/' . $name . '.rdf';
        } else
        {
            $xml_file = SF_WEB_PATH . '/syn/' . $GLOBALS['xml_file'];
            $rdf_file = SF_WEB_PATH . '/syn/' . $GLOBALS['rdf_file'];
            $rdf_url = SF_ROOT_URL . '/syn/' . $GLOBALS['rdf_file'];
        }

        $site_nick = $GLOBALS['site_nick'];
		if (file_exists( SF_INCLUDE_PATH . '/rss_description.inc'))
		{
			$site_name = file_get_contents( SF_INCLUDE_PATH . '/rss_description.inc');
		}
		else
		{
			$site_name = $GLOBALS['site_name'];
		}

        $root_url = SF_ROOT_URL.'/';
        $news_url = SF_NEWS_URL.'/';
        $uploads_url = SF_UPLOAD_URL.'/';
        $rss=new \RSSWriter($root_url, $site_nick, $site_name, $rdf_url, array("dc:publisher" => $site_nick, "dc:creator" => $site_nick));
        $rss->useModule("content", "http://purl.org/rss/1.0/modules/content/");
        $rss->useModule("dcterms", "http://purl.org/dc/terms/");
        $rss->setImage($xml_logo, $site_nick);

	$temprs = array_reverse($resultset);
        foreach($temprs as $row) 
        {

            $id = $row['id'];
            $imagelink = "";
            $topic = "";

            if (preg_match("/image/",$row['mime_type']))
            {
                $imagebase = basename(trim($row['linked_file']));
                $imagelink = $uploads_url . $imagebase;
            }

            $created = $row['created'];
	    $date_array = $created; // i guess created is stored in the proper format -jk
            # $date_array  = substr($created,0,4) . "-" . substr($created,4,2) . "-" . substr($created,6,2) . " ";
            # $date_array .= substr($created,8,2) . ":" . substr($created,10,2) . ":" . substr($created,12,2);
            $rdf_date_array = gmdate('Y-m-d\TH:i:s\Z',strtotime($date_array));
            $pathtofile = MakeCacheDir($row['created']);

            $articlelink =  $news_url . $pathtofile . $row['id'] . ".php";
            $db_obj = new \SFACTIVE\DB;
            $catquery = "select c.name as name from catlink l,category c where l.id=$id and c.category_id=l.catid";
            $catset = $db_obj->query($catquery);

            /* cycles through all the category of a article and displays them in the newswire rss feed -- blicero 30.07.2002 */

            while ($catrow = array_pop($catset))
            {
                $topic .= utf8_encode(htmlspecialchars($catrow['name'])) . "\t";
            }

            $topic=trim($topic);

            switch ($row['display'])
            {
                case 'f':
                    $section=utf8_encode(htmlspecialchars($GLOBALS['dict']['status_hidden']));
                    break;
                case 'l':    
                    $section=utf8_encode(htmlspecialchars($GLOBALS['dict']['status_local']));
                    break;
                case 't':
                    $section=utf8_encode(htmlspecialchars($GLOBALS['dict']['status_display']));
                    break;
                case 'g':
                    $section=utf8_encode(htmlspecialchars($GLOBALS['dict']['status_global']));
                    break;
            }
            $heading = trim(utf8_encode(htmlspecialchars($row['heading'])));
            $author= trim(utf8_encode(htmlspecialchars($row['author'])));
            $fields = str_replace(array("&amp;#","&amp;amp;","&amp;gt;","&amp;lt;","&amp;quot;"),array("&#","&amp;","&gt;","&lt;","&quot;"),array($heading,$author,$topic,$section));
            $xml_list .= "
        <story>
            <title>".$fields[0]."</title>
            <url>$articlelink</url>
            <time>$date_array</time>
            <author>".$fields[1]."</author>
            <department>".$row['mime_type']."</department>
            <topic>$fields[2]</topic>
            <comments>".$row['numcomment']."</comments>
            <section>".$fields[3]."</section>
            <image>$imagelink</image>
        </story>";
            if(preg_match('#'.SF_UPLOAD_PATH.'#', $row['linked_file']))
            {
		// note: this will only work if you never moved your images across the filesystem.
                $file = str_replace(SF_UPLOAD_PATH, SF_UPLOAD_URL, $row['linked_file']) ;
            }else {
                $file = '';
            }
            $title = trim(htmlspecialchars($row['heading'], ENT_QUOTES));
            $summary = trim(htmlspecialchars($row['summary'], ENT_QUOTES));
            $date = $rdf_date_array;
            $mime = trim($row['mime_type']);
            $author = trim(htmlspecialchars($row['author'], ENT_QUOTES));
			// echo '['.$row['artmime'].']';
			if ('t'==$row['artmime']) {
					$article = str_replace(array('&lt;', '&gt;','&amp;', '&quot;'), array('<', '>','&', '"'), nl2br(htmlentities($row['article'],ENT_QUOTES,'UTF-8')));
			} else {
					$article = str_replace(array('&lt;', '&gt;','&amp;', '&quot;'), array('<', '>','&', '"'), htmlentities($row['article'],ENT_QUOTES,'UTF-8'));
			}
			$language = trim(utf8_encode(htmlspecialchars($lang_obj->get_language_code($row['language_id']))));
			if (preg_match('/mp3$/', $file))
			{
				$size = filesize($row['linked_file']);
				$rss->addItem($articlelink, $title, array("description" => $summary, "dc:date" => $date, "dc:subject" => $name, "dc:creator" => $author, "dc:format" => $mime, "dc:language" => $language, "content:encoded" => $article, "dcterms:hasPart" => $file, 'enclosure url="'.$file.'" length="'.$size.'" type="audio/mpeg"'=>'' ));
			}
			else
			{
				$rss->addItem($articlelink, $title, array("description" => $summary, "dc:date" => $date, "dc:subject" => $name, "dc:creator" => $author, "dc:format" => $mime, "dc:language" => $language, "content:encoded" => $article, "dcterms:hasPart" => $file ));
			}
        }

        $xml = "";
        $xml = "<?xml version=\"1.0\"?>
        <!DOCTYPE backslash [ <!ELEMENT backslash (story*)> <!ELEMENT story (title, url, time, author, department, topic, comments, section, image)> <!ELEMENT title (#PCDATA)> <!ELEMENT url (#PCDATA)> <!ELEMENT time (#PCDATA)> <!ELEMENT author (#PCDATA)> <!ELEMENT department (#PCDATA)> <!ELEMENT topic (#PCDATA)> <!ELEMENT comments (#PCDATA)> <!ELEMENT section (#PCDATA)> <!ELEMENT image (#PCDATA)> ]>
        <backslash>";
        $xml .= $xml_list;
        $xml .= "
        </backslash>";
        $rdf=$rss->serialize();
        $rdf=str_replace(array("&amp;#","&amp;amp;","&amp;gt;","&amp;lt;","&amp;quot;"),array("&#","&amp;","&gt;","&lt;","&quot;"),$rdf);
        $fffp = fopen($xml_file, "w");
        fwrite($fffp, $xml, strlen($xml));
        fclose($fffp);
        $fffp = fopen($rdf_file, "w");
        fwrite($fffp, $rdf, strlen($rdf));
        fclose($fffp);
    }

    function get_max_post_id(){
	$db_obj= new \SFACTIVE\DB;
	$return_val=0;
	$result=$db_obj->query("Select max(id) as id from webcast where parent_id=0");
	$row=array_pop($result);
	$t_return_val=$row["id"];
	if (strlen($t_return_val)>0){
		$return_val=$t_return_val;
	}	
	return $return_val;
    }

    function get_max_id()
    {
	$db_obj = new \SFACTIVE\DB;
	$result = $db_obj->query('select max(id) as id from webcast');
	$row = array_pop($result);
	$this->max_id = $row['id'];
	return $this->max_id ;
    }

    function get_max_cat_id(){
	$db_obj= new \SFACTIVE\DB;
	$return_val=0;
	$result=$db_obj->query("Select max(id) as id from catlink");
	$row=array_pop($result);
	$t_return_val=$row["id"];
	if (strlen($t_return_val)>0){
		$return_val=$t_return_val;
	}	
	return $return_val;
    }

    function get_max_comment_id(){
	$db_obj= new \SFACTIVE\DB;
	$return_val=0;
	$result=$db_obj->query("Select max(id) as id from webcast where parent_id>0");
	$row=array_pop($result);
	$t_return_val=$row["id"];
	if (strlen($t_return_val)>0){
		$return_val=$t_return_val;
	}	
	return $return_val;
    }

    function get_id_list($min_id, $num_rows, $where){
	$db_obj=new \SFACTIVE\DB;
	$sql="Select id from webcast where id>$min_id $where order by id limit $num_rows";
	echo $sql;
	$result=$db_obj->query($sql);
	return $result;
    }

}
