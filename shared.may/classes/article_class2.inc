<?php

// commented out. i got troubles with one of the cli_'s with this.
// include_once("shared/global.cfg");
include_once(SF_SHARED_PATH."/classes/image_class.inc");
define(COMMENT_CACHE_LIMIT, 2048);

function compare_by_id( $a, $b ) { return strcmp($a['id'],$b['id']); }

class Article extends Cache
{
    //The article class represents a post in the system. It should contain all of the code
    //related to the manipulation of posts.

    var $article;                 // An associative array of the article's data
    var $comments;                // A database resultset of the article's comments
    var $attachments;             // A database resultset of the article's attachments

    var $html_article;            // Rendered HTML of the article body
    var $html_comments;           // Rendered HTML of the article's comments
    var $html_comments_table;     // Rendered HTML of the article's latest comments table

    function Article ($article_id=FALSE)
    {
        // The article constructor will set all values if it gets a valid article id
        if ($article_id !== FALSE)
        {
            if ($this->set_article_data($article_id))
            {
                $this->set_comments_data("asc",10);
                $this->set_attachments_data();
                $this->set_mime_type();
            } else
            {
                // error handling, could not instantiate class
            }
        }
    }

    function set_article_data ($article_id = 0)
    {
        //returns all the fields for a given post from the post's id
        global $time_zone;
        global $time_diff;
        $db_obj = new DB;

        // Set up timezone per local configuration
        if (!isset($time_zone)) $time_zone=date(Z);
        if (!isset($time_diff)) $time_diff=0;

        if (strlen($article_id) > 0)
        {
            $query = "SELECT w.id as id,
                          w.heading as heading,
                          w.author as author,
                          date_format((w.created + interval $time_diff second), '%W, %b. %d, %Y at %l:%i %p') as format_created,
                          date_format((w.modified - interval ".$time_zone." second), '%a, %d %b %Y %H:%i:%s') as last_modified,
                          date_format((w.modified + interval ".$time_diff." second), '%W, %b. %d, %Y at %l:%i %p') as format_modified,
                          w.article as article,
                          w.display as display,
                          w.contact as contact,w.link as link,w.address as address,
                          w.phone as phone,
                          w.parent_id as parent_id,
                          w.mime_type as mime_type,
                          w.summary as summary,
                          w.numcomment as numcomment,
                          w.arttype as arttype,
                          w.html_file as html_file,
                          w.modified as modified,
                          w.created as created,
                          w.linked_file as linked_file,
                          w.artmime as artmime,
                          date_format(w.created,'%Y') as created_year,
                          date_format(w.created, '%m') as created_month,
						  w.language_id as language_idi,  
						  w.rating as rating  
                         FROM webcast w WHERE w.id=
                         $article_id";

            $resultset = $db_obj->query($query);

            // Now set up all the variables for the article
            $this->article = array_pop($resultset);
            $this->set_paths();

            return 1;
        } else
        {
            return 0;
        }
    }

    function set_article_data_from_form ()
    {
        // Instead of the database, set the article properties from a form POST
        $arttype = "webcast";
        $parent_id = 0;
        $numcomments = 0;
        $htmlfilename = chop($texttype);
        $tb_name="webcast";
        $seq_name="webcastid";

        if ($_FILES["linked_file_1"]["name"]!="")
        {

            $mime_type = $_POST["mime_type_file_1"];
            $target_url = $this->upload_target_url[1];

        } else if (strlen($_POST["linked_file"])>0)
        {
            $mime_type = $_POST["mime_type"];
            $target_url = $_POST["linked_file"];
        } else
        {
            if ($_POST["artmime"] == 'h')
            {
                $mime_type="text/html";
            } else
            {
                $mime_type="text/plain";
            }
            $target_url = "";
        }

        $this->article["heading"] =     $_POST[heading];
        $this->article["author"] =      $_POST[author];
        $this->article["article"] =     $_POST[article];
        $this->article["artmime"] =     $artmime;
        $this->article["contact"] =     $_POST[contact];
        $this->article["link"] =        $_POST[link];
        $this->article["address"] =     $_POST[address];
        $this->article["phone"] =       $_POST[phone];
        $this->article["parent_id"] =   $parent_id;
        $this->article["mime_type"] =   $mime_type;
        $this->article["summary"] =     $_POST[summary];
        $this->article["numcomment"] =  $numcomments;
        $this->article["arttype"] =     $arttype;
        $this->article["artmime"] =     $_POST[artmime];
        $this->article["html_file"] =   $htmlfilename;
        $this->article["linked_file"] = $target_url;
        $this->article["table_name"] =  $tb_name;
        $this->article["seq_name"] =    $seq_name;
        $this->article["display"] =     "t";
	$this->article["language_id"] =	$_POST['language_id'];

//echo "in set_article_data_from_form";
//echo $this->article["article"];

        /* checking if validation was selected*/
        if($_POST["contact"] && $_POST["validate"]){
	// can't do this here, don't have an article_id yet
        // $this->set_validation_hash();
	   $this->article["validate"] = 1;
        }
        else{
	// is this necessary?
            $this->article["validate"] = 0;
        }

        return 1;
    }

    function set_comments_data ($order, $limit)
    {
        // Returns comment data for this article
        global $time_zone;
        global $time_diff;
        $db_obj = new DB;

        // Set up timezone per local configuration
        if (!isset($time_zone)) $time_zone=date(Z);
        if (!isset($time_diff)) $time_diff=0;

        $limitquery = "";

        if (strtoupper($order) != "DESC") $order = "ASC";
        if ($limit > 0) $limitquery = "LIMIT $limit";

        	$query = "SELECT w.id as id,
                     w.heading as heading,
                     w.author as author,
                     date_format((w.created + interval $time_diff second), '%W, %b. %d, %Y at %l:%i %p') as format_created,
                     date_format((w.modified - interval ".$time_zone." second), '%a, %d %b %Y %H:%i:%s') as last_modified,
                     date_format((w.modified + interval ".$time_diff." second), '%W, %b. %d, %Y at %l:%i %p') as format_modified,
                     w.article as article,
                     w.contact as contact,
                     w.link as link,
                     w.address as address,
                     w.phone as phone,
                     w.parent_id as parent_id,
                     w.mime_type as mime_type,
                     w.summary as summary,
                     w.numcomment as numcomment,
                     w.arttype as arttype,
                     w.html_file as html_file,
                     w.modified as modified,
                     w.created as created,
                     w.linked_file as linked_file,
                     w.artmime as artmime,
                     w.display AS display,
				     w.language_id as language_id,
				     w.rating as rating
                    FROM webcast w WHERE w.parent_id=".
                    $this->article['id'];
		
        	if (!$_GET[hidden]) $query .= " AND display!='f'";

 	  		$query .= " and arttype!='attachment' ORDER BY id ASC ";
			$this->comments = $db_obj->query($query);

        return 1;
    }

    function set_attachments_data ()
    {
        // Sets $this->attachments
        $db_obj = new DB;

        $query = "SELECT w.id as id,
                     w.heading as heading,
                     w.article as article,
                     w.author as author,
                     w.parent_id as parent_id,
                     w.mime_type as mime_type,
                     w.arttype as arttype,
                     w.html_file as html_file,
                     w.linked_file as linked_file,
                     w.artmime as artmime,
                     w.display as display,
		     w.language_id as language_id,
		     w.rating as rating
                    FROM webcast w WHERE w.parent_id=".
                    $this->article['id'];

        if (!$_GET[hidden]) $query .= " AND display!='f'";

        $query .= " and arttype='attachment' ORDER BY id ASC";

        $this->attachments = $db_obj->query($query);
        return 1;
    }

    function set_paths ()
    {
        // Sets the class properties related to filesystem or url paths

        $this->article['basename'] = basename($this->article["linked_file"]);
        if(ereg($this->article["created_year"] . "/" . $this->article["created_month"], $this->article["linked_file"]))
        {
            $this->article['filepath'] = SF_UPLOAD_PATH . "/" . $this->article["created_year"] . "/" . $this->article["created_month"] . "/" . $this->article["basename"];
            $this->article['fileurl']  = SF_UPLOAD_URL  . "/" . $this->article["created_year"] . "/" . $this->article["created_month"] . "/" . $this->article["basename"];
        }else
        {
	    $this->article['filepath'] = SF_UPLOAD_PATH . "/" . $this->article["basename"];
	    $this->article['fileurl']  = SF_UPLOAD_URL  . "/" . $this->article["basename"];
	}

        // This sets up the filesystem/url paths for this post
        $this->article['articlepath'] =    SF_NEWS_PATH . "/" . $this->article["created_year"];
        $this->article['articlepath'] .=   "/" . $this->article["created_month"] . "/";

        $this->article['article_url'] =    SF_NEWS_URL . "/" . $this->article["created_year"];
        $this->article['article_url'] .=   "/" . $this->article["created_month"] . "/";
        $this->article['article_url_path'] = $this->article['article_url'];

        $this->article['article_url'] .=   $this->article["id"] . ".php";

        // This creates the filesize HTML

        if (strlen($this->article["linked_file"]) > 0)
        {
	    if (file_exists($this->article["filepath"])){
            if (filesize($this->article["filepath"]))
            {
                $this->render_file_size();
            } else
            {
                $this->article["filesize"] = "";
            }
	    }
        }

        return 1;
    }

    function set_mime_type ()
    {
        // Sets the appropriate mime types and makes custom field changes

        $tr = new Translate;

        switch ($this->article[mime_type])
        {
            case "text/plain":
            case "text/html":
                $this->article["mime_description"] = $tr->trans('html_article');
                $this->article["media"] .= "";
                if (strlen($this->article["linked_file"]) > 0)
                {
                    if (file_exists($this->article["filepath"]))
                    {
                        $fd = fopen($this->article["filepath"], "r");
			$file_contents = fread($fd,filesize($this->article["filepath"]));
                        fclose($fd);
                        $file_contents = stripslashes($file_contents);
			if ($this->article["mime_type"]=='text/html')
                        {
$this->article['uploaded_article'] = $this->cleanup_html($file_contents);
                        } else {
$this->article['uploaded_article'] = $this->cleanup_text($file_contents);
                        }
                    }
                }
                break;

            case "image/jpeg":
            case "image/gif":
            case "image/png":
            case "application/x-shockwave-flash":
		$this->article['media'] = '';
                if (file_exists($this->article["filepath"])){
		$this->article["mime_description"] = $tr->trans('image_file');
		// echo $this->article["filepath"].'<br />';
                $image_info = getimagesize($this->article["filepath"]);
                $image_size = $image_info[0] . "x" . $image_info[1];
                $image_alt  = substr($this->article["heading"],0,20) . "...";
                if ($image_info['mime']) { $image_type = $image_info['mime']; }
                else { $image_type = $this->article["mime_type"]; }
                eregi("([a-z0-9/\._-]+)\.([a-z0-9]+)$", $this->article['fileurl'], $regs);
                $upload_ext = $regs[2];

		if (file_exists($this->article['filepath']."mid.".$upload_ext)) {
                    $this->article['fileurl_mid'] = $this->article['fileurl']."mid.".$upload_ext;
                    $gonna_linking=1;
                    $image_info = getimagesize($this->article['filepath']."mid.".$upload_ext);
                }else
                { 
		    $this->article['fileurl_mid'] = $this->article['fileurl']; 
		}

                if ($gonna_linking) $this->article['media'] ="<a href=".$this->article['fileurl']." target=\"new\">";

                if ($image_info[2] == 4)
                {
                    $this->article["media"] .= "\n<embed ";
                } else
                {
                    $this->article["media"] .= "\n<img itemprop=\"images\" ";
                }

                $this->article["media"] .= 'src="' . $this->article["fileurl_mid"] . '" ';
                $this->article["media"] .= $image_info[3] . ' alt="' . $image_alt . '" border="0" />';
                $this->article["media"] .= "<br />" . $this->article["basename"] . ", ";
                $this->article["media"] .= $image_type . ", " . $image_size . "";
		if ($gonna_linking) $this->article['media'] .= "</a>";

                }
		break;

            case "application/pdf":
                $this->article["mime_description"] = $tr->trans('pdf_file');
                $this->article["media"]  = "<a href=\"" . $this->article["fileurl"];
                $this->article["media"] .= "\">download PDF (" . $this->article["filesize"] . ")</a>";
                break;

            case "application/smil":
                $this->article["mime_description"] = $tr->trans('smil_file');
                $this->article["media"] .= "multimedia: <a href=\"" . $this->article["fileurl"] . "\">SMIL at ";
                $this->article["media"] .= $this->article["filesize"] . "</a>";
                break;

            case "audio/mpeg":
                $this->article["mime_description"] = $tr->trans('mp3_file');
                $this->article["media"] .= "audio: <a href=\"" . $this->article["fileurl"] . "\">MP3 at ";
                $this->article["media"] .= $this->article["filesize"] . "</a>";

				if (file_exists(SF_TEMPLATE_PATH.'/media-audio-mpeg.tpl') && $this->article['parent_id']==0)
				{
					$template_obj = new FastTemplate(SF_TEMPLATE_PATH);
					$template_obj->clear_all();
					$template_obj->define(array(article_page => 'media-audio-mpeg.tpl'));
					$defaults = array('U_RL' => $this->article['fileurl']);
					$template_obj->assign($defaults);
					$template_obj->parse(CONTENT,"article_page");
					$this->article['media'] = $template_obj->fetch("CONTENT");
				}
                break;

            case "audio/x-ogg":
                $this->article["mime_description"] = 'ogg vorbis';
                $this->article["media"] .= "audio: <a href=\"" . $this->article["fileurl"] . "\">ogg vorbis at ";
                $this->article["media"] .= $this->article["filesize"] . "</a>";
                break;

            case "video/x-ogg":
                $this->article["mime_description"] = 'ogg vorbis';
                $this->article["media"] .= "video: <a href=\"" . $this->article["fileurl"] . "\">ogg vorbis at ";
                $this->article["media"] .= $this->article["filesize"] . "</a>";
                break;

            case "audio/x-pn-realaudio":
                $this->article["mime_description"] = $tr->trans('realaudio_file');
                $ramfilebase = eregi_replace(".r[avm]$", ".ram", $this->article["basename"]);
                $ramurl  = $GLOBALS["upload_url"] . $ramfilebase;
                $rmurl    = $GLOBALS["upload_url"] . $this->article["basename"];

                $this->article["media"] .= "RealAudio: <a href=\"" . $ramurl . "\">stream ";
                $this->article["media"] .= "with RealPlayer</a><small>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;or ";
                $this->article["media"] .= "<a href=\"" . $rmurl . "\">download RM file</a> (";
                $this->article["media"] .= $this->article["filesize"] . ")</small>";
                break;

            case "audio/x-mpegurl":
            case "audio/mpegurl":
                $this->article["mime_description"] = $tr->trans('mp3_stream');
                $this->article["media"] .= "audio: <a href=\"" . $this->article["fileurl"] . "\">MP3 stream (mpegurl)</a>";
                break;

            case "audio/x-scpls":
                $this->article["mime_description"] = $tr->trans('mp3_playlist');
                $this->article["media"] .= "audio: <a href=\"" . $this->article["fileurl"] . "\">MP3 [playlist]</a>";
                break;

            case "audio/x-pn-realaudio-meta":
                $this->article["mime_description"] = $tr->trans('realaudio_metafile');
                $this->article["media"] .= "audio: <a href=\"" . $this->article["fileurl"] . "\">RealAudio ";
                $this->article["media"] .= "metafile</a>";
                break;

            case "audio/x-ms-wma":
                $this->article["mime_description"] = $tr->trans('wm_audio_file');
                $this->article["media"] .= "audio: <a href=\"" . $this->article["fileurl"] . "\">windows ";
                $this->article["media"] .= "media at " . $this->article["filesize"] . "</a>";
                break;

            case "video/mpeg":
                $this->article["mime_description"] = $tr->trans('mp3_file');
                $url_title = urlencode(addslashes($this->article["heading"]));
                $url_src   = urlencode($this->article["fileurl"]);
                $jss_url   = SF_NEWS_URL."/embed.php?src=" . $url_src . "&amp;title=" . $url_title;
              	$this->article["media"] .= "video: <a href=\"javascript: window.open('" . $jss_url . "',''";
              	$this->article["media"] .= ",'scrollbars,width=600,height=600,resizable'); void('');\">QuickTime Popup Player</a> : ";
                $this->article["media"] .= "video: <a href=\"" . $this->article["fileurl"] . "\">MPEG at ";
                $this->article["media"] .= $this->article["filesize"] . "</a>";
                break;

            case "video/x-pn-realvideo":
                $this->article["mime_description"] = $tr->trans('realvideo_file');
                $ramfilebase = eregi_replace(".r[avm]$", ".ram", $this->article["basename"]);
                $ramurl  = $GLOBALS["upload_url"] . $ramfilebase;
                $rmurl    = $GLOBALS["upload_url"] . $this->article["basename"];

                $this->article["media"] .= "RealVideo: <a href=\"" . $ramurl . "\">stream ";
                $this->article["media"] .= "with RealPlayer</a><small>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;or ";
                $this->article["media"] .= "<a href=\"" . $rmurl . "\">download RM file</a> (";
                $this->article["media"] .= $this->article["filesize"] . ")</small>";
                break;

            case "video/quicktime":
                $this->article["mime_description"] = $tr->trans('quicktime_file');
                $url_title = urlencode(addslashes($this->article["heading"]));
                $url_src   = urlencode($this->article["fileurl"]);
                $jss_url   = SF_NEWS_URL."/embed.php?src=" . $url_src . "&amp;title=" . $url_title;
                $this->article["media"] .= "video: <a href=\"javascript: window.open('" . $jss_url . "',''";
                $this->article["media"] .= ",'scrollbars,width=600,height=600,resizable'); void('');\">QuickTime Popup Player</a> :";
                $this->article["media"] = "<a href=\"" . $this->article["fileurl"] . "\">QuickTime movie at ";
                $this->article["media"] .= $this->article["filesize"] . "</a>";
                break;

            case "video/x-msvideo":
                $this->article["mime_description"] = $tr->trans('avi_file');
                $this->article["media"] .= "video: <a href=\"" . $this->article["fileurl"] . "\">AVI ";
                $this->article["media"] .= "at " . $this->article["filesize"] . "</a>";
                break;

            case "video/x-pn-realvideo-meta":
                $this->article["mime_description"] = $tr->trans('realvideo_metafile');
                $this->article["media"] .= "video: <a href=\"" . $this->article["fileurl"] . "\">RealVideo ";
                $this->article["media"] .= "metafile</a>";
                break;

            case "video/x-ms-wmv":
                $this->article["mime_description"] = $tr->trans('wm_file');
                $this->article["media"] .= "video: <a href=\"" . $this->article["fileurl"] . "\">windows ";
                $this->article["media"] .= "media at " . $this->article["filesize"] . "</a>";
                break;

            case "audio/x-wav":
		$this->article["mime_description"] = $tr->trans('wav file');
		$this->article["media"] .= "audio: <a href=\"" . $this->article['fileurl'] . "\">wav ";
		$this->article["media"] .= "file (" . $this->article['filesize'] . ")</a>";
		break;

            case "application/x-bittorrent":
                $this->article["mime_description"] = 'BitTorrent';
                $this->article["media"] .= "download: <a href=\"" . $this->article["fileurl"] . "\">BitTorrent ";
                $this->article["media"] .= "at " . $this->article["filesize"] . "</a>";
                break;

	    case "application/rtf":
		$this->article['mime_description'] = 'Rtf';
		$this->article['media'] .="download: <a href=\"".$this->article['fileurl']."\">Rtf ";
		$this->article['media'] .="at " . $this->article['filesize']."</a>";
		break;
        }
    }

    function update_from_array ($update)
    {
        $db_obj = new DB;
        $validkeys = array("heading","author","article","contact","link","mime_type","artmime",
                           "summary","address","display","arttype","phone","parent_id","linked_file","status", "language_id");

        if ($update['id'] > 0)
        {
            $count = 0;
            $query = "UPDATE webcast SET ";

            while (list($key, $value) = each ($update))
            {
                if (in_array($key,$validkeys))
                {
                     if ($count > 0) $query .= ",";
                     $query .= "$key = '" . $value . "'";
                     $count++;
                }
            }     

            $query .= " WHERE id=" . $update['id'];
            $db_obj->execute_statement($query);
        }

        if ($update['parent_id'] != 0)
        {
            $this->Article($update['parent_id']);
        } else
        {
            $this->Article($update['id']);
        }

        $this->render_everything();
        $this->cache_to_disk();

        return 1;
    }

    function update ()
    {
        // Updates the article in the database
        $db_obj = new DB;

        if (strlen($this->article) > 0)
        {
            $query = "UPDATE webcast 
                        SET heading = '"    .$this->article[heading]."',".
                        "author='"          .$this->article[author]."',".
                        "article='"         .($this->article[article])."',".
                        "contact='"         .$this->article[contact]."',".
                        "link='"            .$this->article[link]."',".
                        "mime_type='"       .$this->article[mime_type]."',".
                        "artmime='"         .$this->article[artmime]."',".
                        "summary='"         .($this->article[summary])."',".
                        "address='"         .$this->article[address]."',".
                        "display='"         .$this->article[display]."',".
                        "arttype='"         .$this->article[arttype]."',".
                        "phone='"           .$this->article[phone]."',".
                        "parent_id='"       .$this->article[parent_id]."',".
                        "linked_file='"     .$this->article[linked_file]."', ".
			"validate='"        .$this->article[validate]."' ,".
			"language_id='"	    .$this->article[language_id].",";

            $query .= " WHERE id=" . $this->article[id];
            $db_obj->execute_statement($query);
            $this->set_article_data($this->article[id]);
            $this->render_everything();
            $this->cache_to_disk();
            return 1;
        }
    }

    function render ()
    {

        // Renders a post
        $db_obj = new DB;
	$tr = new Translate ;
	include_once(SF_CACHE_PATH.'/language_options.inc');
        $template_obj = new FastTemplate(SF_TEMPLATE_PATH);

        // Prevent the article from being rendered if it is hidden
        if ($this->article[display] == "f")
        {
            $template_name = "hidden";
			// also escape text so links break
			$this->article['article'] = htmlspecialchars($this->article['article']);
        } else
        {
            $template_name = "text-html";
        }

        // Set up the template
        $template_name .= ".tpl";
        $template_obj->clear_all();
        $template_obj->define(array(article_page => $template_name));

        // Set up template variables
        $url_author = urlencode($this->article["author"]);
		if ($this->article['display']=='f') $this->article['link'] = preg_replace('/\\./', '. ', $this->article['link']); 
        $urllink = chop($this->article["link"]);

        if (!(eregi("^http://|^ftp://|^https://",$urllink,$reg)))
        {
            $urllink = "http://" . $urllink;
        }

        $croplink = eregi_replace("^http://", "", $urllink);

        if (strlen($croplink) > 80) $croplink = substr($croplink, 0, 77) . "...";

        if ($this->article["artmime"] == "h")
        {
		    $this->article["article"]=$this->cleanup_html($this->article["article"]);
			if ($this->article['display']=='f') 
				$this->article["article"]=$this->damage_links($this->article["article"]);
        } else {
		    $this->article["article"]=$this->cleanup_text($this->article["article"]);
			if ($this->article['display']<>'f') 
				$this->article["article"]=$this->link_text($this->article["article"]);
			else
				$this->article["article"]=$this->damage_links($this->article["article"]);
        }

		$this->article["article"] .= $this->article['uploaded_article'];
        // Assign the template variables
        $defaults=(array(   H_EADING  =>   $this->article["heading"],
                            A_UTHOR   =>   $this->article["author"],
                            U_RLAUTH  =>   "$url_author",
                            C_DATE    =>   $this->article["format_created"],
                            A_DDRESS  =>   $this->article["address"],
                            P_HONE    =>   $this->article["phone"],
                            M_EDIA    =>   $this->article["media"],
                            S_UMMARY  =>   $this->article["summary"],
                            C_ONTACT  =>   $this->article["contact"],
                            L_INK     =>   "$urllink",
                            C_ROPURL  =>   "$croplink",
                            A_RTID    =>   $this->article["id"],
                            A_RTICLE  =>   $this->article["article"],
                            R_ATING   =>   floor($this->article["rating"]),
			    V_ALIDATED => "",
			    L_ANGUAGE =>   $language_options[$this->article[language_id]],
                            C_ANCHOR  =>   $this->article["id"],
							TPL_PAGE_TITLE => urlencode($this->article["heading"]),
							TPL_SHORT_URL => "/display.php?id=".$this->article["id"]
							));

        if ($this->article["parent_id"] != 0) $defaults['A_RTID'] = $this->article["parent_id"];   
      if ($this->article[contact]) {
          if ( $this->check_validation() )
              $defaults[V_ALIDATED]= $tr->trans('article_validated'); 
          else 
              $defaults[V_ALIDATED]= $tr->trans('article_not_validated'); 
        }

	// Generate an image attachment preview.
	if ( $_POST['regenerate']=='true' and substr($this->article['mime_type'],0,5) == 'image' ) 
	{
		$linked_file = $this->article['linked_file'];
		$path_parts = pathinfo( $linked_file );
		$image=new Image();
		$image->validate_image( $linked_file, $path_parts['extension'] );
	}

        // Process the template
        $template_obj->assign($defaults);
        $template_obj->parse(CONTENT,"article_page");
        $result_html = $template_obj->fetch("CONTENT");

        if ($this->article[parent_id] == 0) $GLOBALS["numcomment_parent"] = $this->article[numcomment];
        $this->html_article = $result_html;
    }

    function render_summary ()
    {
        //renders a post as html given teh post's fields
        $db_obj = new DB;
        $template_obj = new FastTemplate(SF_TEMPLATE_PATH);

        // Set up the template
        $template_name = "text-html";
        $template_name .= ".tpl";
        $template_obj->clear_all();

        // Set up the template variables
        $template_obj->define(array(article_page => $template_name));

        $url_author = urlencode($this->article[author]);
        $urllink = chop($this->article[link]);

        if (!(eregi("^http://|^ftp://|^https://",$urllink,$reg)))
        {
            $urllink = "http://" . $urllink; 
        }

        $croplink = eregi_replace("^http://", "", $urllink);
        if (strlen($croplink) > 80) $croplink = substr($croplink, 0, 77) . "...";

        // Assign the template variables
        $defaults=(array(   H_EADING  =>   "$this->article[heading]",
                              A_UTHOR   =>   "$this->article[author]",
                              U_RLAUTH  =>   "$url_author",
                              C_DATE    =>   "$this->article[format_created]",
                              A_DDRESS  =>   "$this->article[address]",
                              P_HONE    =>   "$this->article[phone]",
                              M_EDIA    =>   "",
                              S_UMMARY  =>   "$this->article[summary]",
                              C_ONTACT  =>   "$this->article[contact]",
                              L_INK     =>   "",
                              C_ROPURL  =>   "",
                              A_RTID    =>   "",
                              A_RTICLE  =>   "",
                              C_ANCHOR  =>   ""));

        // Process the template
        $template_obj->assign($defaults);
        $template_obj->parse(CONTENT,"article_page");

        $result_html = $template_obj->fetch("CONTENT");
        $this->html_summary = $result_html;
    }

    function render_all_comments ()
    {
        // Renders all commments as HTML for a given post
        global $shared_scripts_path;

        $this->set_comments_data("asc",0);

        if (is_array($this->comments))
        {
            while($article_fields = array_pop($this->comments))
            {
                $comment = new Article($article_fields["id"]);
                if($GLOBALS['move_attachments'])
                {
                    $comment->handle_move_uploads();
                    $comment->set_paths();
                    $comment->set_mime_type();
                }
                $comment->render();
                $this->html_comments .= $comment->html_article;
            }
        }
    }

    function render_attachments ()
    {
        // Renders all attachments as HTML for a given post
        global $shared_scripts_path;

        $this->set_attachments_data();

        if (is_array($this->attachments))
        {
            $attachments = $this->attachments;
            while ($article_fields = array_pop($attachments))
            {
                $attachment = new Article($article_fields["id"]);
                if($GLOBALS['move_attachments'])
                {
                    $attachment->handle_move_uploads();
                    $attachment->set_paths();
                    $attachment->set_mime_type();
                }
                $return_html .= $attachment->render();
                $this->html_attachments .= $attachment->html_article;
            }
        }
    }

    function render_comments_list ()
    {
        // This creates a comment list for use on the article
        global $shared_scripts_path;
        global $template_obj;
        $template_obj = new FastTemplate(SF_TEMPLATE_PATH);

        // Set how many last comments to show at once
        $ccount = 10;

        $this->set_comments_data("desc", 10);

        // Set up the templates
        $template_obj->clear_all();
        $template_obj->define(array(comments_list_top     =>   "comments_list_top.tpl"));
        $template_obj->define(array(comments_list_middle  =>   "comments_list_middle.tpl"));
        $template_obj->define(array(comments_list_bottom  =>   "comments_list_bottom.tpl"));
        $defaults = (array(N_UMCOMMENT     =>   "$GLOBALS[numcomment_parent]"));

        $template_obj->assign($defaults);

        // Process the top of the latest comments table
        $template_obj->parse(CONTENT,"comments_list_top");
        $result_html = $template_obj->fetch("CONTENT");

        // Process a row for each comment which is being displayed
        if (is_array($this->comments))
        {
            $comments = $this->comments;
            while($comment = array_pop($comments))
            {
               $rep = $this->article['id'] . ".php";
               $cl = ereg_replace("$rep","",$this->article['article_url']);
               $croplink = $cl . $this->article['id'] . "_comment.php#" . $comment[id];
               $mime_obj->article_fields[anchor] = $mime_obj->article_fields[id];
               $mime_obj->article_fields[id] = $mime_obj->article_fields[parent_id];

                // Assign template variables
                $defaults=(array(   H_EADING  =>   "$comment[heading]",
                                    A_UTHOR   =>   "$comment[author]",
                                    U_RLAUTH  =>   "$url_author",
                                    C_DATE    =>   "$comment[format_created]",
                                    C_ROPURL  =>   "$croplink",
                                    A_RTID    =>   "$comment[id]",
                                    A_RTICLE  =>   "$comment[article]",
                                    C_ANCHOR  =>   "$comment[id]"));

                // Process the template
                $template_obj->assign($defaults);
                $template_obj->parse(CONTENT,"comments_list_middle");
                $result_html .= $template_obj->fetch("CONTENT");
            }
        }

        // Process the bottom of the latest comments table
        $template_obj->assign($defaults);
        $template_obj->parse(CONTENT,"comments_list_bottom");
        $result_html .= $template_obj->fetch("CONTENT");

        $this->html_comments_table = $result_html;
    }

    function render_file_size ()
    {
        // Returns HTML for a filesize

        $size = filesize($this->article["filepath"]);
        if ($size < 1048576)
        {
            $size = $size / 1024;
            $size = sprintf("%.1f",$size);
            $size = $size . " kibibytes";
        } else
        {
            $size = $size / 1048576;
            $size = sprintf("%.1f",$size);
            $size = $size . " mebibytes";
        }

        $this->article["filesize"] = $size;
        return 1;
    }

    function render_everything ()
    {
        // This renders all parts of a post
        
        if ($this->article[parent_id]!=0)
        {
            $this->set_article_data($this->article[parent_id]);
        } else
        {
            $this->set_article_data($this->article[id]);
        }

        $this->set_mime_type ();

        $this->render();
        $this->render_summary();
        $this->render_attachments();
        $this->render_all_comments();
        $this->render_comments_list();
        $Cat = new Category;
        $Cat->get_article_catname_array($this->article['id']);

        $headerstring  = "<?php\ninclude(\"shared/global.cfg\");\n";
        $headerstring .= "header(\"Last-Modified: " . gmdate("D, d M Y H:i:s",time()) . " GMT\");\n\n";
        $headerstring .= "\$GLOBALS['page_title'] ='";
        $headerstring .= str_replace(array('"',"'"),array('&quot;',"\'"),$this->article['heading']);
        $headerstring .= "';\n";
        $headerstring .= "\$GLOBALS['page_description'] ='";
        $headerstring .= str_replace(array('"',"'","\n","\r"),array('&quot;',"\'",' ',''),$this->article['summary']);
        $headerstring .= "';\n";
        $headerstring .= "\$GLOBALS['page_id']    =" . $this->article['id'] . ";\n";
        $headerstring .= '$GLOBALS["page_display"] = "' . $this->article['display'] . '";' . "\n";
			$headerstring .= $Cat->cat_name_match_printable . "\n\n";
        $headerstring .= "sf_include_file(SF_INCLUDE_"."PATH, \"/article_header.inc\");\n?" . ">\n\n";

        $this->html_header = $headerstring;

        $this->html_footer = "\n<?php sf_include_file(SF_INCLUDE_"."PATH, \"/article_footer.inc\"); ?" . ">";
        $commentstring="";  
     
        return 1;
    }

    function insert ()
    {
        // Adds an article to the DB
        $db_obj = new DB();

        // If this is a comment, increment the numcomment count for the parent article
        if ($this->article["parent_id"] != 0 && $this->article['arttype'] !== "attachment")
        {
			/* this is bad. we don't even have to select this
            $query_string="SELECT numcomment 
                              FROM webcast
                              WHERE id = " . $this->article["parent_id"];

            $result=$db_obj->query($query_string);
            $row = array_pop($result);
            $nc = $row["numcomment"];

            if ($this->article["arttype"] != "attachment") $nc++; */

            $query_string="UPDATE webcast
                              SET numcomment= numcomment +1,
                                  modified=now()
                              WHERE id=" . $this->article["parent_id"];

            $result = $db_obj->execute_statement($query_string);
        }

        // Create the INSERT query
        $query = "INSERT INTO webcast (heading, author, contact, link, address, 
                     phone, mime_type, summary, numcomment, arttype, artmime, html_file, mirrored, 
                     display, linked_file, created, modified, id, article, parent_id, rating, language_id)
                     VALUES ('"
                         . $this->article["heading"] . "','"
                         . $this->article["author"] . "','"
                         . $this->article["contact"] . "','"
                         . $this->article["link"] . "','"
                         . $this->article["address"] . "','"
                         . $this->article["phone"] . "','"
                         . $this->article["mime_type"] . "','"
                         . ($this->article["summary"]) . "','"
                         . $this->article["numcomment"] . "','"
                         . $this->article["arttype"] . "','"
                         . $this->article["artmime"] . "','"
                         . $this->article["html_file"] . "','"
                         . $this->article["mirrored"] . "','"
                         . $this->article["display"] . "','"
                         . $this->article["linked_file"] . "',now(), now(), NULL,'"
                         . ($this->article["article"]) . "',"
                         . $this->article["parent_id"] . ",NULL, '"
//			 . $this->article["validate"] . "', '"
			 . $this->article["language_id"] . "')";

//echo "in insert";
//echo $this->article["article"];
//echo "<p>$query";
        // Execute the query
        $this->article["id"] = $db_obj->execute_statement_return_autokey($query);
        $this->set_paths();
	
	if ($this->article["validate"] = 1)
	{
	  $this->set_validation_hash();
	}
	
        return 1;
    }

    function cache_to_disk ()
    {
        $tr = new Translate;

        $nocomment_link  = "<br /><a href=\"" . $this->article['id'] . ".php\">";
        $nocomment_link .= $tr->trans('view_article_wo_comment') . "</a>";

        $filedata = $this->html_header;
        $filedata_comments = $filedata . $nocomment_link . $this->html_article . $this->html_attachments;

        // Build the HTML for the normal article page
// build the comment box where is needed . new config file entry to do it added by blicero

        if (eregi($this->article["display"],$GLOBALS["boxed"])) {
            $filedata .= $this->html_article . $this->html_attachments;
            if ($this->article["numcomment"] > 0)
            {
                $filedata .= $this->html_comments_table;
            } else
            {
                $filedata .= $this->html_comments;
            }
        }else{
           $filedata .= $this->html_article . $this->html_attachments . $this->html_comments;
            $filedata_comments .= "<a name=\"begin_comments\"></a>\n";
        }

        $filedata .= $this->html_footer;
        $filedata_comments .= $this->html_comments;
        $filedata_comments .= $this->html_footer;

		$reversed_attachments = $this->attachments; // copy array
		usort( $reversed_attachments, "compare_by_id" ); 

		$reversed_comments = $this->comments;
		usort( $reversed_comments, "compare_by_id" ); 

        $filedata_json = json_encode(array(
								"article"=>$this->article,
								"attachments"=>$reversed_attachments,
								"comments"=>$reversed_comments));
        // really ugly hack to fix paths -johnk
        $filedata_json = str_replace( "\/usr\/local\/sf-active\/la.indymedia.org\/public\/",
                                      "http:\/\/la.indymedia.org\/\/", $filedata_json );

        // Now we cache the article
        if (!is_dir(SF_NEWS_PATH . "/" . $this->article['created_year']))
        {
            mkdir(SF_NEWS_PATH . "/" . $this->article['created_year'],0777);
            chmod(SF_NEWS_PATH . "/" . $this->article['created_year'],0777);
        }

        if (!is_dir($this->article['articlepath']))
        {
            mkdir($this->article['articlepath'], 0777);
        }

        $article_filename = $this->article['articlepath'] . $this->article['id'] . ".php";
        $comment_filename = $this->article['articlepath'] . $this->article['id'] . "_comment.php";
        $json_filename = $this->article['articlepath'] . $this->article['id'] . ".json";
		# echo "$article_filename <p>";

        $this->cache_file($filedata, $article_filename);
        $this->cache_file($filedata_comments, $comment_filename);
        $this->cache_file($filedata_json, $json_filename);

        $tmpupd  = $tr->trans('your_story_is') . " <a href=".$this->article['article_url'].">";
        $tmpupd .= $this->article['article_url'] . "</a>";
        $this->update_status($tmpupd);
        return 1;
    }

    function get_post_ids_starting_with ($start_id, $numtogen)
    {
        // Process article id's for use with the regeneration code
        $db_obj = new DB;        
        $sql = "select id from webcast where parent_id=0 and id>=".$start_id." ORDER by id LIMIT 0, ".$numtogen;
        return $db_obj->query($sql);
    }

    function cleanup_text($tmpvar)
    {
        // Process text articles, adds url's, etc
        $tmpvar = stripslashes($tmpvar);

        // replace newlines with html
        $tmpvar = eregi_replace("(\n\n|\r\r|\r\n\r\n|\n\t|\r\t\|\r\n\t)", " <br /><br /> ", $tmpvar);
        $tmpvar = eregi_replace("(\n|\r|\r\n)", " <br /> ", $tmpvar);

        return $tmpvar;
    }

	function link_text($tmpvar) 
	{
        // encode
        $tmpvar = str_replace(array('<','>','"'),array('&lt;','&gt;','&quot;'),$tmpvar);
        
        // re-encode to avoid confusing the link generator
        $tmpvar = str_replace(array('&quot;','&lt;','&gt;'),array(' &quot; ',' &lt; ',' &gt; '),$tmpvar);

        // turn e-mail addresses into mailto links
        $tmpvar = eregi_replace('[A-Za-z0-9_]([-._]?[A-Za-z0-9])*@[A-Za-z0-9]([-.]?[A-Za-z0-9])*\.[A-Za-z]+', '<a href="mailto:\\0">\\0</a>', $tmpvar);

        // turn URLs into links; 
        $tmpvar = eregi_replace("((www\.)([a-zA-Z0-9@:%_.~#-\?&]+[a-zA-Z0-9@:%_~#\?&/]))", "http://\\1", $tmpvar);
        $tmpvar = eregi_replace("((ftp://|http://|https://){2})([a-zA-Z0-9@:%_.~#-\?&]+[a-zA-Z0-9@:%_~#\?&/])", "http://\\3", $tmpvar);
        $tmpvar = eregi_replace("(((ftp://|http://|https://){1})[a-zA-Z0-9@:%_.~#-\?&]+[a-zA-Z0-9@:%_~#\?&/])", "<a href=\"\\1\" rel=\"nofollow\">\\1</a>", $tmpvar);

        // turn <a href="<a href=""></a>"> and <a href=<a href=""></a>> back into links
        $tmpvar = eregi_replace("(( &lt; a href= &quot; <a href=\")(((ftp://|http://|https://){1})[a-zA-Z0-9@:%_.~#-\?&]+[a-zA-Z0-9@:%_~#\?&/]\">))((((ftp://|http://|https://)([a-zA-Z0-9@:%_.~#-\?&]+[a-zA-Z0-9@:%_~#\?&/]))))(</a> &quot;  &gt; )", "<a href=\"\\6\">", $tmpvar); 
        $tmpvar = eregi_replace("(( &lt; a href=<a href=\")(((ftp://|http://|https://){1})[a-zA-Z0-9@:%_.~#-\?&]+[a-zA-Z0-9@:%_~#\?&/]\">))((((ftp://|http://|https://)([a-zA-Z0-9@:%_.~#-\?&]+[a-zA-Z0-9@:%_~#\?&/]))))(</a> &gt; )", "<a href=\"\\6\">", $tmpvar);
        $tmpvar = eregi_replace(" &lt; /a &gt; ", "</a>", $tmpvar);

        // de-reencode
        $tmpvar = str_replace(array(' &lt; ',' &gt; ',' &quot; '), array('&lt;','&gt;','&quot;'),$tmpvar); 
        $tmpvar = str_replace(array('&lt;','&gt;','&quot;'), array('<','>','"'),$tmpvar); 
		return $tmpvar;
	}
	function damage_links($text)
	{
		$text = preg_replace_callback('/\\s\\.+?.com/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
		$text = preg_replace_callback('/www\\.+?\\s/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
		$text = preg_replace_callback('/www\\.+?$/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
		$text = preg_replace_callback('/www\\.+?[>.]/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
		$text = preg_replace_callback('/http:\\/\\/.+?\\s/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
		$text = preg_replace_callback('/http:\\/\\/.+?$/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
		$text = preg_replace_callback('/http:\\/\\/.+?[>.]/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
		return $text;
	}

    function cleanup_html ($tmpvar)
    {
        // Removes unwanted HTML from the article
        $tmpvar = eregi_replace("(<applet|<body|<head|<html|<link|<meta|<object|<script|<iframe|<font)", "<null", $tmpvar);
        $tmpvar = eregi_replace("(</applet|</body|</head|</html|</link|</meta|</object|</script|</iframe|</font)","</null",$tmpvar);
        $tmpvar = eregi_replace('<title>','<!-- ',$tmpvar);
        $tmpvar = eregi_replace('</title>',' -->',$tmpvar);
        $tmpvar = str_replace(array('<?','?>','&quot;', '<%', '%>'),array('<!--','-->','"','<!--','-->'),$tmpvar);
        $tmpvar = eregi_replace('(class|color|onblur|onchange|onclick|ondblclick|onfocus|onkeydown|onkeypress|onkeyup|onload|onmousedown|onmousemove|onmouseout|onmouseover|onmouseup|onreset|onselect|onsubmit|onunload)="[^"]+"', " ", $tmpvar);
        $tmpvar = eregi_replace('(class|color|onblur|onchange|onclick|ondblclick|onfocus|onkeydown|onkeypress|onkeyup|onload|onmousedown|onmousemove|onmouseout|onmouseover|onmouseup|onreset|onselect|onsubmit|onunload)=[^\>]+\>', ">", $tmpvar);
        return $tmpvar;
    }

    function update_article_status ($id, $new_status)
    {
        $db_obj = new DB; 
        $query = "UPDATE webcast SET display='$new_status' where id=$id";
        $db_obj->execute_statement($query);
		// seems excessive.  but also seems necessary.
        $this->set_article_data($id);

		// this block appears to be redundant.  see render_everything();
        //if ($this->article['parent_id']>0)
        //{
        //    $this->set_article_data($this->article['parent_id']);
        //}

        $this->render_everything();
        $this->cache_to_disk();
        return 1;
    }
    function handle_move_uploads()
    {
        // this should be set configurable.
        // this function should be able to move uploaded files to yyyy/mm directories when regenerating articles.
        // should eventually resize the images as well ?
        // should be moved to article_class


        if(strlen($this->article["linked_file"]) < 1 || ereg("/".$this->article["created_year"]."/".$this->article["created_month"]."/", $this->article["linked_file"]))
        {
                        if(!is_array($this->article))
                        {
                                return "no article data" ;
                        }else{
                return 'nothing to move' ;
                        }
        }else
        {
            // let's start with creating the upload directories
            $this->create_upload_dirs();


            // we move the file
            $file = basename($this->article['linked_file']);
            $new_path = SF_UPLOAD_PATH."/".$this->article["created_year"]."/".$this->article["created_month"];
            $new_location = $new_path ."/" . $file ;
            if(file_exists($new_location))
            {
                // here we prevent overwriting.
                $tmp = tempnam($new_path, $file);
                unlink($tmp);
                $new_location = strtolower($tmp);
            }
            $old_location = SF_UPLOAD_PATH . "/".$file ;
            if(rename($old_location, $new_location))
            {
                                // we do the resizing from here.
                                include_once(SF_CLASS_PATH.'/image_class.inc');
                                $image = new Image ;
                                $changed_image = $image->change_size_from_old($new_location, $this->article['created_year'], $this->article['created_month']);
//print($changed_image);
                                if($changed_image !== 0)
                                {
                                        $new_location = $changed_image;
                                }

                // we update the dbase.
                $this->article["linked_file"] = $new_location ;
                $db_obj = new DB ;
                $sql = "update webcast set linked_file = '".$this->article["linked_file"]."' where id = '".$this->article["id"]."'";
                $db_obj->execute_statement($sql);
                return "moved the file";
            }else
            {
                return "move failed for: ".$this->article['linked_file'] ;
            }
        }

    }
	function resize_image_attachment()
	{

	}

	function check_validation()
	{
	// see if an article have been validated by email, return true if so

		$db_obj = new DB;

                $query = "SELECT validated FROM validation where article_id = " . $this->article["id"]  ;
                $resultset = $db_obj->query($query);

		// do we really need to put this in the array?
		$this->article["is_validated"] = $resultset['validated'];
		if (is_array($resultset)) $row = array_pop($resultset);

		if ($row['validated'] == 't') return 1;
		else return 0;
	}

	function set_validation_hash()
	{

	    $db_obj = new DB;

            /* creating hash for email validation */
            srand((double)microtime()*1000000);
            $hash  = md5(rand(0,9999999).time());
            /*
               this should insert a 32 char varchar into the validation table
               for this article_id in the database. this can then be compared to the
               information submitted in the validation phase

	       this is better than storing everything in the webcast table!!
            */

	    $query = "INSERT INTO validation (article_id, validated, hash) VALUES (" . $this->article["id"] . ", 'f', '" . $hash . "' ) " ;
	    $db_obj->execute_statement($query);

//            $this->article["validate"] = $hash;
//               echo "set_validation_hash  " . $hash ;
	}

	function get_validation_hash()
	{
	   $db_obj = new DB;

	   $query = "SELECT hash from validation WHERE article_id = ". $this->article["id"] ;

           $resultset = $db_obj->query($query);
	   $row = array_pop($resultset);

	   return $row['hash'];
	}

	function confirm_email_validation()
	{
	  $db_obj = new DB;
	  $query = "UPDATE validation SET validated = 't' WHERE article_id = ". $this->article["id"] ;
	  $db_obj->execute_statement($query);

	}


}


