<?php  // vim:et:sw=4:ts=4:st=4:

namespace SFACTIVE;

include_once(SF_SHARED_PATH."/classes/image_class.inc");
include_once(SF_SHARED_PATH."/classes/image2_class.inc");
include_once(SF_SHARED_PATH."/classes/json_article.inc");
define('COMMENT_CACHE_LIMIT', 2048);


class Article extends \Cache
{
    //The article class represents a post in the system. It should contain all of the code
    //related to the manipulation of posts.

    var $article;                 // An associative array of the article's data
    var $comments;                // A database resultset of the article's comments
    var $attachments;             // A database resultset of the article's attachments

    var $html_article;            // Rendered HTML of the article body
    var $html_comments;           // Rendered HTML of the article's comments
    var $html_comments_table;     // Rendered HTML of the article's latest comments table

    var $has_dumped_webcast_latest;  // used to avoid dumping the db twice

    var $foobar;

    function __construct($article_id=FALSE)
    {

        $this->has_dumped_webcast_latest = FALSE;
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
                throw new \Exception('Could not find article');
            }
        }
        $this->foobar = $article_id;
        return $this;
    }
    function update_webcast_indexes() {
        $db_obj = new \SFACTIVE\DB;
        $d = $this->article['display'];
        $id = $this->article['id'];
        //echo "<p>update_webcast_indexes id=$id display=$d</p>";
        // fixme - add code to check current display, and exit if it's the same
        if ($d=='l') {
            $db_obj->execute("DELETE FROM webcast_parents_t WHERE id=$id");
            $db_obj->execute("INSERT INTO webcast_parents_l SELECT * FROM webcast WHERE id=$id");
            $db_obj->execute("INSERT INTO webcast_parents SELECT * FROM webcast WHERE id=$id ON DUPLICATE KEYS UPDATE webcast_parents.display=webcast.display");
        }
        if ($d=='t') {
            $db_obj->execute("DELETE FROM webcast_parents_l WHERE id=$id");
            $db_obj->execute("INSERT INTO webcast_parents_t SELECT * FROM webcast WHERE id=$id");
            $db_obj->execute("INSERT INTO webcast_parents SELECT * FROM webcast WHERE id=$id ON DUPLICATE KEYS UPDATE webcast_parents.display=webcast.display");
        }
        if ($d=='f') {
            $db_obj->execute("DELETE FROM webcast_parents_l WHERE id=$id");
            $db_obj->execute("DELETE FROM webcast_parents_t WHERE id=$id");
            $db_obj->execute("DELETE FROM webcast_parents WHERE id=$id");
        }
    }
    function dump_webcast_latest() 
    {
        /* 
           fixme - this always dumps.  instead, it should check if the ID of the article
           is before the lowest ID in the webcast_latest.  If it is, then we should not dump.
        */
        if ($this->has_dumped_webcast_latest) {
            // log an error here
        }

        $db_obj = new DB;
        $db_obj->execute("START TRANSACTION");
        $db_obj->execute("DROP TABLE webcast_latest");
        $db_obj->execute("CREATE TABLE webcast_latest SELECT * FROM `webcast` ORDER BY id DESC LIMIT 1000");
        $db_obj->execute("COMMIT");
        $this->has_dumped_webcast_latest = TRUE;
            //var_dump(debug_backtrace());
        //echo "<p>dumped webcast_latest</p>";
    }

    function dump_article($id) 
    {
        $id = filter_var( $id, FILTER_VALIDATE_INT );
        if (! $this->is_in_webcast_latest($id)) 
        {
            $db_obj = new DB;
            $db_obj->execute("INSERT INTO webcast_latest SELECT * FROM webcast WHERE id=$id");
            $db_obj->execute("INSERT INTO webcast_latest SELECT * FROM webcast WHERE parent_id=$id");
        }
    }
    function is_in_webcast_latest( $id ) 
    {
        $id = filter_var( (int) $id, FILTER_VALIDATE_INT );
        $db_obj = new DB;
        $resultset = $db_obj->query("SELECT id FROM webcast_latest WHERE id=$id");
        return ($resultset[0]==$id);
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
            // check that the article exists
            $query = "SELECT id FROM webcast WHERE id=$article_id";
            $resultset = $db_obj->query($query);
            if (count($resultset)<>1) return 0;

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
            echo "<pre>"; 
            debug_print_backtrace();
            die("Article called with a bad id, $article_id");
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

        $this->article["heading"] =     $_POST['heading'];
        $this->article["author"] =      $_POST['author'];
        $this->article["article"] =     $_POST['article'];
        $this->article["artmime"] =     $artmime;
        $this->article["contact"] =     $_POST['contact'];
        $this->article["link"] =        $_POST['link'];
        $this->article["address"] =     $_POST['address'];
        $this->article["phone"] =       $_POST['phone'];
        $this->article["parent_id"] =   $parent_id;
        $this->article["mime_type"] =   $mime_type;
        $this->article["summary"] =     $_POST['summary'];
        $this->article["numcomment"] =  $numcomments;
        $this->article["arttype"] =     $arttype;
        $this->article["artmime"] =     $_POST['artmime'];
        $this->article["html_file"] =   $htmlfilename;
        $this->article["linked_file"] = $target_url;
        $this->article["table_name"] =  $tb_name;
        $this->article["seq_name"] =    $seq_name;
        $this->article["display"] =     "t";
        $this->article["language_id"] = $_POST['language_id'];

        // if they include the string idVer in the author,
        // replace it with "spoof attempt of"
        $this->article['author'] = str_replace('idVer', 'Spoof Attempt of', $this->article['author']);

        // some magic to make an ID Key from a secret
        if (isset($_POST['secret'])) {
            $secret = $_POST['secret'];
            $author = $this->article['author'];
            if ($secret and $author) {
                $salt = 'fy29hgfqlibfqiyr3bireh2iouvqyhiey8o3ynfcuiehirhew3iqr';
                $hash = sha1(hash('sha256',hash('sha256',$secret.$author.$salt).$salt).$salt);
                $this->article['author'] = "$author idVer:$hash";
            }
        }
        // disallow non-ascii characters
        $this->article['author'] = filter_var( $this->article['author'],
           FILTER_SANITIZE_STRING,
           FILTER_FLAG_STRIP_LOW|FILTER_FLAG_STRIP_HIGH );

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

        //$this->dump_article($this->article['id']);
        if ($this->is_in_webcast_latest( $this->article['id'] )) {
            $webcast = 'webcast_latest';
        } else {
            $webcast = 'webcast';
        }

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
                    FROM $webcast w WHERE w.parent_id=".
                    $this->article['id'];
        
            if (!$_GET['hidden']) $query .= " AND display!='f'";

            $query .= " and arttype!='attachment' ORDER BY id ASC ";
            $this->comments = $db_obj->query($query);

        return 1;
    }

    function set_attachments_data ()
    {
        // Sets $this->attachments
        $db_obj = new DB;

        //$this->dump_article($this->article['id']);
        if ($this->is_in_webcast_latest( $this->article['id'] )) {
            $webcast = 'webcast_latest';
        } else {
            $webcast = 'webcast';
        }

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
                    FROM $webcast w WHERE w.parent_id=".
                    $this->article['id'];

        if (!$_GET['hidden']) $query .= " AND display!='f'";

        $query .= " and arttype='attachment' ORDER BY id ASC";

        $this->attachments = $db_obj->query($query);
        return 1;
    }

    function set_paths ()
    {
        // Sets the class properties related to filesystem or url paths

        $this->article['basename'] = basename($this->article["linked_file"]);
        $year = $this->article["created_year"];
        $month = $this->article["created_month"];
        if(preg_match("/$year\/$month/", $this->article["linked_file"]))
        {
            $this->article['filepath'] = SF_UPLOAD_PATH . "/$year/$month/" . $this->article["basename"];
            $this->article['fileurl']  = SF_UPLOAD_URL  . "/$year/$month/" . $this->article["basename"];
        }
        else
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

        $tr = new \Translate;

        switch ($this->article['mime_type'])
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
                        if ($this->article["mime_type"]=='text/html') {
                            $this->article['uploaded_article'] = $this->force_nofollow($this->cleanup_html($file_contents));
                        } else {
                            $this->article['uploaded_article'] = $this->force_nofollow($this->cleanup_text($file_contents));
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
                    preg_match('/([a-z0-9\/\._-]+)\.([a-z0-9]+)$/i', $this->article['fileurl'], $regs);
                    $upload_ext = $regs[2];

                    if (file_exists($this->article['filepath']."mid.".$upload_ext)) {
                        $this->article['fileurl_mid'] = $this->article['fileurl']."mid.".$upload_ext;
                        $gonna_linking=1;
                        $image_info = getimagesize($this->article['filepath']."mid.".$upload_ext);
                    } else { 
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
                    $template_obj = new \FastTemplate(SF_TEMPLATE_PATH);
                    $template_obj->clear_all();
                    $template_obj->define(array('article_page' => 'media-audio-mpeg.tpl'));
                    $defaults = array('U_RL' => $this->article['fileurl']);
                    $template_obj->assign($defaults);
                    $template_obj->parse('CONTENT',"article_page");
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
                $ramfilebase = preg_replace("/.r[avm]$/", ".ram", $this->article["basename"]);
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
                $this->article["media"] .= "<video controls width='600'>";
                $this->article["media"] .= "  <source src='".$this->article['fileurl']."' type='video/mp4'>";
                $this->article["media"] .= "</video><br />";
                $this->article["media"] .= "video: <a href=\"" . $this->article["fileurl"] . "\">MPEG at ";
                $this->article["media"] .= $this->article["filesize"] . "</a>";
                break;

            case "video/x-pn-realvideo":
                $this->article["mime_description"] = $tr->trans('realvideo_file');
                $ramfilebase = preg_replace("/\.r[avm]$/", ".ram", $this->article["basename"]);
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
            $db_obj->execute($query);
            $this->dump_webcast_latest();
            
            if ($update['display']) {
                $this->__construct($update['id']);
                $this->update_webcast_indexes();
            }
        }

        if ($update['parent_id'] != 0)
        {
            $this->__construct($update['parent_id']);
        } else
        {
            $this->__construct($update['id']);
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
                        SET heading = '"    .$this->article['heading']."',".
                        "author='"          .$this->article['author']."',".
                        "article='"         .($this->article['article'])."',".
                        "contact='"         .$this->article['contact']."',".
                        "link='"            .$this->article['link']."',".
                        "mime_type='"       .$this->article['mime_type']."',".
                        "artmime='"         .$this->article['artmime']."',".
                        "summary='"         .($this->article['summary'])."',".
                        "address='"         .$this->article['address']."',".
                        "display='"         .$this->article['display']."',".
                        "arttype='"         .$this->article['arttype']."',".
                        "phone='"           .$this->article['phone']."',".
                        "parent_id='"       .$this->article['parent_id']."',".
                        "linked_file='"     .$this->article['linked_file']."', ".
            "validate='"        .$this->article['validate']."' ,".
            "language_id='"     .$this->article['language_id'].",";

            $query .= " WHERE id=" . $this->article['id'];
            $db_obj->execute($query);
            $this->set_article_data($this->article['id']);
            $this->render_everything();
            $this->cache_to_disk();
            return 1;
        }
    }

    function render ()
    {

        // Renders a post
        $db_obj = new DB;
        $tr = new \Translate ;
        include_once(SF_CACHE_PATH.'/language_options.inc');
        $template_obj = new \FastTemplate(SF_TEMPLATE_PATH);

        // Prevent the article from being rendered if it is hidden
        if ($this->article['display'] == "f")
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
        $template_obj->define(array('article_page' => $template_name));

        // Set up template variables
        $url_author = urlencode($this->article["author"]);
        if ($this->article['display']=='f') $this->article['link'] = preg_replace('/\\./', '. ', $this->article['link']); 
        $urllink = chop($this->article["link"]);

        $urllink = $this->prepend_http($urllink);

        $croplink = preg_replace("/^http:\/\//i", "", $urllink);

        if (strlen($croplink) > 60) $croplink = substr($croplink, 0, 57) . "...";

        if ($this->article["artmime"] == "h")
        {
            $this->article["article"]=$this->force_nofollow($this->cleanup_html($this->article["article"]));
            if ($this->article['display']=='f') 
            {
                $this->article["article"]=$this->damage_links($this->article["article"]);
                $croplink = base64_encode($croplink);
                $this->article['link']=base64_encode($this->article['link']);
            }
        } else {
            $this->article["article"]=$this->cleanup_text($this->article["article"]);
            if ($this->article['display']=='f') 
            {
                $this->article["article"]=$this->damage_links($this->article["article"]);
                $this->article['link']=base64_encode($this->article['link']);
                $croplink = base64_encode($croplink);
            }
            else
            {
                $this->article["article"]=$this->link_text($this->article["article"]);
            }
        }
            

        $this->article["article"] .= $this->article['uploaded_article'];

        // local articles have followable links to pass authority
		if ($this->article['display']=='l') {
			$this->article['article'] = $this->remove_nofollow($this->article['article']);
		}

        // Assign the template variables
        $defaults=(array(   'H_EADING'  =>   $this->article["heading"],
                            'A_UTHOR'   =>   $this->article["author"],
                            'U_RLAUTH'  =>   "$url_author",
                            'C_DATE'    =>   $this->article["format_created"],
                            'A_DDRESS'  =>   $this->article["address"],
                            'P_HONE'    =>   $this->article["phone"],
                            'M_EDIA'    =>   $this->article["media"],
                            'S_UMMARY'  =>   $this->article["summary"],
                            'C_ONTACT'  =>   $this->article["contact"],
                            'L_INK'     =>   "$urllink",
                            'C_ROPURL'  =>   "$croplink",
                            'A_RTID'    =>   $this->article["id"],
                            'A_RTICLE'  =>   $this->article["article"],
                            'R_ATING'   =>   floor($this->article["rating"]),
                            'V_ALIDATED' => "",
                            'L_ANGUAGE' =>   $language_options[$this->article['language_id']],
                            'C_ANCHOR'  =>   $this->article["id"],
                            'TPL_PAGE_TITLE' => urlencode($this->article["heading"]),
                            'TPL_SHORT_URL' => "/display.php?id=".$this->article["id"]
                            ));

        // local articles have followable links to pass authority
		if ($this->article['display']=='l') {
			$defaults['N_FOLLOW'] = 'dofollow';
		} else {
			$defaults['N_FOLLOW'] = 'nofollow';
        }

        if ($this->article["parent_id"] != 0) $defaults['A_RTID'] = $this->article["parent_id"];   
        if ($this->article['contact']) {
            if ( $this->check_validation() )
                $defaults['V_ALIDATED']= $tr->trans('article_validated'); 
            else 
                $defaults['V_ALIDATED']= $tr->trans('article_not_validated'); 
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
        $template_obj->parse('CONTENT',"article_page");
        $result_html = $template_obj->fetch("CONTENT");

        if ($this->article['parent_id'] == 0) $GLOBALS["numcomment_parent"] = $this->article['numcomment'];
        $this->html_article = $result_html;
    }

    private function prepend_http($urllink) 
    {
        if (!preg_match("/^(http|ftp|https):/", $urllink, $reg))
        {
            $urllink = "http://" . $urllink;
        }
        return $urllink;
    }

    function render_summary ()
    {
        //renders a post as html given teh post's fields
        $db_obj = new DB;
        $template_obj = new \FastTemplate(SF_TEMPLATE_PATH);

        // Set up the template
        $template_name = "text-html";
        $template_name .= ".tpl";
        $template_obj->clear_all();

        // Set up the template variables
        $template_obj->define(array('article_page' => $template_name));

        $url_author = urlencode($this->article['author']);
        $urllink = chop($this->article['link']);

        $urllink = $this->prepend_http($urllink);

        $croplink = preg_replace("#^http://#", "", $urllink);

        if (strlen($croplink) > 60) $croplink = substr($croplink, 0, 57) . "...";

        // Assign the template variables
        $defaults=(array(     'H_EADING'  =>   $this->article["heading"],
                              'A_UTHOR'   =>   $this->article["author"],
                              'U_RLAUTH'  =>   $url_author,
                              'C_DATE'    =>   $this->article["format_created"],
                              'A_DDRESS'  =>   $this->article["address"],
                              'P_HONE'    =>   $this->article["phone"],
                              'M_EDIA'    =>   "",
                              'S_UMMARY'  =>   $this->article["summary"],
                              'C_ONTACT'  =>   $this->article["contact"],
                              'L_INK'     =>   "",
                              'C_ROPURL'  =>   "",
                              'A_RTID'    =>   "",
                              'A_RTICLE'  =>   "",
                              'C_ANCHOR'  =>   ""));

        // Process the template
        $template_obj->assign($defaults);
        $template_obj->parse('CONTENT',"article_page");

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
        $template_obj = new \FastTemplate(SF_TEMPLATE_PATH);

        // Set how many last comments to show at once
        $ccount = 10;

        $this->set_comments_data("desc", 10);

        // Set up the templates
        $template_obj->clear_all();
        $template_obj->define(array('comments_list_top'     =>   "comments_list_top.tpl"));
        $template_obj->define(array('comments_list_middle'  =>   "comments_list_middle.tpl"));
        $template_obj->define(array('comments_list_bottom'  =>   "comments_list_bottom.tpl"));
        $defaults = (array('N_UMCOMMENT'     =>   $GLOBALS['numcomment_parent']));

        $template_obj->assign($defaults);

        // Process the top of the latest comments table
        $template_obj->parse('CONTENT',"comments_list_top");
        $result_html = $template_obj->fetch("CONTENT");

        // Process a row for each comment which is being displayed
        if (is_array($this->comments))
        {
            $comments = $this->comments;
            while($comment = array_pop($comments))
            {
               $rep = $this->article['id'] . ".php";
               $cl = preg_replace("/$rep/","",$this->article['article_url']);
               $croplink = $cl . $this->article['id'] . "_comment.php#" . $comment['id'];
               $mime_obj->article_fields['anchor'] = $mime_obj->article_fields['id'];
               $mime_obj->article_fields['id'] = $mime_obj->article_fields['parent_id'];

                // Assign template variables
                $defaults=(array(   'H_EADING'  =>   $comment['heading'],
                                    'A_UTHOR'   =>   $comment['author'],
                                    'U_RLAUTH'  =>   $url_author,
                                    'C_DATE'    =>   $comment['format_created'],
                                    'C_ROPURL'  =>   $croplink,
                                    'A_RTID'    =>   $comment['id'],
                                    'A_RTICLE'  =>   $comment['article'],
                                    'C_ANCHOR'  =>   $comment['id']));

                // Process the template
                $template_obj->assign($defaults);
                $template_obj->parse('CONTENT',"comments_list_middle");
                $result_html .= $template_obj->fetch("CONTENT");
            }
        }

        // Process the bottom of the latest comments table
        $template_obj->assign($defaults);
        $template_obj->parse('CONTENT',"comments_list_bottom");
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

    function set_canonical_url() {
    $this->canonical_url = SF_NEWS_URL.'/'.$this->article['created_year'].'/'.$this->article['created_month'].'/'.$this->article['id'].'.php';
    }


    function render_everything ()
    {
        // This renders all parts of a post
        
        if ($this->article['parent_id']!=0)
        {
            $this->set_article_data($this->article['parent_id']);
        } else
        {
            $this->set_article_data($this->article['id']);
        }

        $this->set_canonical_url();

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
        $headerstring .= $this->escape_quotes_newlines($this->article['heading']);
        $headerstring .= "';\n";
        $headerstring .= "\$GLOBALS['page_description'] ='";
        $headerstring .= $this->escape_quotes_newlines($this->article['summary']);
        $headerstring .= "';\n";
        $headerstring .= "\$GLOBALS['page_id']    =" . $this->article['id'] . ";\n";
        $headerstring .= '$GLOBALS["page_display"] = "' . $this->article['display'] . '";' . "\n";
        $headerstring .= '$GLOBALS["canonical_url"] = "' . $this->canonical_url . '";' . "\n";
        $headerstring .= $Cat->cat_name_match_printable . "\n\n";
        $headerstring .= "sf_include_file(SF_INCLUDE_"."PATH, \"/article_header.inc\");\n?" . ">\n\n";

        $this->html_header = $headerstring;

        $this->html_footer = "\n<?php sf_include_file(SF_INCLUDE_"."PATH, \"/article_footer.inc\"); ?" . ">";
        $commentstring="";  
     
        return 1;
    }

    function escape_quotes_newlines($text) {
        return str_replace(array('"',"'","\n","\r",'\\'),array('&quot;',"&apos;",' ','','\\\\'), $text);
    }

    function insert()
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

            $result = $db_obj->execute($query_string);
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
            //           . $this->article["validate"] . "', '"
                         . $this->article["language_id"] . "')";

        //echo "in insert";
        //echo $this->article["article"];
        //echo "<p>$query";
        // Execute the query
        $this->article["id"] = $db_obj->insert($query);
        $this->set_paths();

        $this->dump_webcast_latest();
        $this->update_webcast_indexes();
    
        if ($this->article["validate"] = 1)
        {
          $this->set_validation_hash();
        }
    
        return 1;
    }

    function cache_to_disk ()
    {
        $tr = new \Translate;

        $canonical_link  = "<a href=\"" . $this->canonical_url . "\">";
        $nocomment_link =  $canonical_link . $tr->trans('view_article_wo_comment') . "</a>";
    $original_link = "Original: " . $canonical_link . htmlspecialchars($this->article["heading"]) . "</a>";

        $filedata = $this->html_header;
        $filedata_comments = $filedata . $nocomment_link . $this->html_article . $this->html_attachments;

        // Build the HTML for the normal article page
    // build the comment box where is needed . new config file entry to do it added by blicero

        if (preg_match('#'.$this->article["display"].'#',$GLOBALS["boxed"])) {
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
                                "article"=>extract_json_article($this->article),
                                "attachments"=>extract_json_comments($reversed_attachments),
                                "comments"=>extract_json_comments($reversed_comments)));
        // really ugly hack to fix paths -johnk
        //  /mnt/ad3/usr/local/www-domains
        $filedata_json = str_replace( "\/mnt\/ad3\/usr\/local\/www-domains\/la.indymedia.org\/public\/",
                                      "http:\/\/la.indymedia.org\/\/", $filedata_json );
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

        # sometimes, this is nothing - not sure when or why but this is here to find it.
        if ($this->article['id']=='') die('the article ID is nothing!');

        $html_filename = $this->article['articlepath'] . $this->article['id'] . ".html";
        $html_comments_filename = $this->article['articlepath'] . $this->article['id'] . "_comment.html";
        $article_filename = $this->article['articlepath'] . $this->article['id'] . ".php";
        $comment_filename = $this->article['articlepath'] . $this->article['id'] . "_comment.php";
        $json_filename = $this->article['articlepath'] . $this->article['id'] . ".json";
        # echo "$article_filename <p>";

        # strip the interactive parts from the html article
        $aa = preg_replace("/<!-- INTERACTIVE -->.*?<!-- \/INTERACTIVE -->/ms", '', $this->html_article);
        $aac = preg_replace("/<!-- INTERACTIVE -->.*?<!-- \/INTERACTIVE -->/ms", '', $this->html_comments);
        $title = htmlspecialchars($this->article['heading']);
        // $htmlheader = "\xEF\xBB\xBF<!doctype html>\r<html><head>\r<meta charset=\"utf-8\" />\r<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r<title>$title</title><link rel=\"stylesheet\" href=\"/css/style.css\"><link rel=\"canonical\" href=\"$this->canonical_url\" /></head><body>";
        $htmlheader = "<!doctype html>\r<html><head>\r<meta charset=\"utf-8\" />\r<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r<title>$title</title><link rel=\"stylesheet\" href=\"/css/style.css\"><link rel=\"canonical\" href=\"$this->canonical_url\" /></head><body>";
        $htmlfooter = "<p>$original_link</p></body></html>";
        if ($this->article['display']!='f') {
            $this->cache_file($htmlheader.$aa.$htmlfooter, $html_filename);
            $this->cache_file($htmlheader.$aac.$htmlfooter, $html_comments_filename);
            $this->cache_file($filedata, $article_filename);
            $this->cache_file($filedata_comments, $comment_filename);
            $this->cache_file($filedata_json, $json_filename);
        } else {
            $this->delete_file($html_filename);
            $this->delete_file($html_comments_filename);
            $this->delete_file($article_filename);
            $this->delete_file($comment_filename);
            $this->delete_file($json_filename);
        }

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
        $tmpvar = preg_replace("/(\r\r \r\r|\n\n \n\n|\r\n\r\n \r\n\r\n|\n\n|\r\r|\r\n\r\n|\n\t|\r\t\|\r\n\t)/", " <br /><br /> ", $tmpvar);
        $tmpvar = preg_replace("/(\n|\r|\r\n)/", " <br /> ", $tmpvar);

        return $tmpvar;
    }

    function link_text2($tmpvar)
    {
        // A less confusing version of link_text1.

        // guard all <a link text:
        $tmpvar = preg_replace_callback('/<a(.+?)<\\/a>/', function ($t) {
                    return ":--@".base64_encode($t[1])."@--:";
                }, $tmpvar);
        if ($tmpvar == NULL) return "";
        $tmpvar = preg_replace('!(http://|https://)([a-zA-Z0-9@:%_.~#\?&/=-]+[a-zA-Z0-9@:%_~#\?&/=-])!i', '<a href="$1$2" rel="nofollow">$1$2</a>', $tmpvar);
        $tmpvar = preg_replace('/[A-Za-z0-9_]([-._]?[A-Za-z0-9])*@[A-Za-z0-9]([-.]?[A-Za-z0-9])*\.[A-Za-z]+/i', '<a href="mailto:\\0">\\0</a>', $tmpvar);
        $tmpvar = preg_replace_callback('/:--@(.+?)@--:/', function ($t) {
                 $dec = base64_decode($t[1]); return "<a$dec</a>";
             }, $tmpvar);
        return $tmpvar;
    }

    function link_text($tmpvar) {
        return $this->force_nofollow($this->link_text2($tmpvar));
    }

    /**
     * Rewrite all the a hrefs to be nofollow.
     * see - https://stackoverflow.com/questions/5037592/how-to-add-rel-nofollow-to-links-with-preg-replace
     * Note - this may break if the mime type is text/html. It could probably use Tidy to clean up
     * the HTML first.
     */
    function force_nofollow($text) {
	if ($text=='') { return $text; }

        $dom = new \DOMDocument();
        $dom->preserveWhitespace = FALSE;
        $last_error_level = \error_reporting(E_ERROR|E_PARSE);
        $dom->loadHTML('<?xml encoding="UTF-8">'.$text);
        \error_reporting($last_error_level);
        foreach($dom->getElementsByTagName('a') as $a) {
            $nofollow = $dom->createTextNode('nofollow');
            $rel = $a->attributes->getNamedItem('rel');
            if ($rel) {
                if ($rel->nodeValue=='nofollow') {
                    // do nothing
                } else {
                    $rel->appendChild($nofollow);
                }
            } else {
                $newrel = $dom->createAttribute('rel');
                $newrel->appendChild($nofollow);
                $a->appendChild($newrel);
            }
        }
        return $this->trim_html_wrapper($dom->saveHTML());
    }
    function trim_html_wrapper($text) {
	$text = preg_replace('/<\!DOCTYPE.+?<body>/s', '', $text);
	$text = preg_replace('/<\/body><\/html>/', '', $text);
	return $text;
    }

    function remove_nofollow($text) {
        $dom = new \DOMDOcument();
        $dom->preserveWhitespace = FALSE;
        $dom->loadHTML($text);
        foreach($dom->getElementsByTagName('a') as $a) {
            $rel = $a->attributes->getNamedItem('rel');
            if ($rel) {
                if ($rel->nodeValue=='nofollow') {
					$a->removeAttribute('rel');
                }
            } 
        }
        return $dom->saveHTML();
    }

    function damage_links($text)
    {
        $text = preg_replace_callback('/\\s\\.+?.com/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/\\s\\.+?.org/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/www\\.+?\\s/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/www\\.+?$/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/www\\.+?[>.]/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/https{0,1}:\\/\\/.+?\\s/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/https{0,1}:\\/\\/.+?$/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/https{0,1}:\\/\\/.+?[>.]/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        return $text;
    }

    function cleanup_html($text) 
    {
        $text = strip_tags($text, '<p><b><i><u><ul><li><ol><table><tr><td><th><hr><strong><em><small><big><super><sub><br><div><span>');
        $text = str_replace(array('<?','?>','&quot;', '<%', '%>'),array('<!--','-->','"','&lt;%','%&gt;'),$text);
        $text = preg_replace('/(class|color|onblur|onchange|onclick|ondblclick|onfocus|onkeydown|onkeypress|onkeyup|onload|onmousedown|onmousemove|onmouseout|onmouseover|onmouseup|onreset|onselect|onsubmit|onunload)="[^"]+"/i', " ", $text);
        $text = preg_replace('/(class|color|onblur|onchange|onclick|ondblclick|onfocus|onkeydown|onkeypress|onkeyup|onload|onmousedown|onmousemove|onmouseout|onmouseover|onmouseup|onreset|onselect|onsubmit|onunload)=[^\>]+\>/i', ">", $text);
        return $text;
    }

    function update_article_status ($id, $new_status)
    {
        $db_obj = new DB; 
        $query = "UPDATE webcast SET display='$new_status' where id=$id";
        $db_obj->execute($query);
        // instead of doing an expensive dump, we just update the fast read-only table :)
        $query = "UPDATE webcast_latest SET display='$new_status' where id=$id";
        $db_obj->execute($query);
        // seems excessive.  but also seems necessary.
        $this->set_article_data($id);

        $this->update_webcast_indexes();

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


        echo "handle_move_uploads()<br>";
        if(strlen($this->article["linked_file"]) < 1 || preg_match("#/".$this->article["created_year"]."/".$this->article["created_month"]."/#", $this->article["linked_file"]))
        {
            if(!is_array($this->article))
            {
                return "no article data" ;
            } else {
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
            echo "moving from $old_loction to $new_location<br>";
            if(move_uploaded_file($old_location, $new_location))
            {
                // we do the resizing from here.
                $image = new Image();
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
                $db_obj->execute($sql);
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
        $db_obj->execute($query);

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
      $db_obj->execute($query);

    }


}


