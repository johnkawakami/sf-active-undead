<?php  // vim:et:sw=4:ts=4:st=4:ai

namespace \LA\Traits;

/**
 * Helper to save an ArticleModel to files.
 *
 * Establishes paths, canonical URLs, url links to files.
 */
class ArticleSaver 
{
    private $article;
    private $id;
    private $filebasename;
    private $filepath;
    private $fileurl;
    private $articlepath;
    private $articleurl;

    function __construct(Article $article)
    {
        $this->article = $article;

        $linked_file = $article->article['linked_file'];
        $file_exists = file_exists($linked_file);
        $year = $article->article["created_year"];
        $month = $article->article["created_month"];

        if ($file_exists) {
            $this->filebasename = basename($article->article["linked_file"]);
            if(preg_match("/$year\/$month/", $this->article["linked_file"]))
            {
                $this->article['filepath'] = SF_UPLOAD_PATH . "/$year/$month/" . $this->basename;
                $this->article['fileurl']  = SF_UPLOAD_URL  . "/$year/$month/" . $this->basename;
            }
            else
            {
                // legacy uploads
                $this->article['filepath'] = SF_UPLOAD_PATH . "/" . $this->basename;
                $this->article['fileurl']  = SF_UPLOAD_URL  . "/" . $this->basename;
            }
            // fixme - move this out of here
            $this->render_file_size();
        }

        // This sets up the filesystem/url paths for this post
        $this->articlepath =   SF_NEWS_PATH . "/$year/$month/";
        $this->articleurlpath =    SF_NEWS_URL . "/$year/$month/"
    }


    /* this was extracted from render() and should be called, explicitly in the article regeneration script. */
    function regenerateImage( array $article )
    {
        // Generate an image attachment preview.
        if ( $_POST['regenerate']=='true' and substr($this->article['mime_type'],0,5) == 'image' ) 
        {
            $linked_file = $this->article['linked_file'];
            $path_parts = pathinfo( $linked_file );
            $image=new Image();
            $image->validate_image( $linked_file, $path_parts['extension'] );
        }
    }

    /* this was extracted from render_attachments() and should be called explicitly from scripts that set move_attachments */
    function moveAttachment( array $article )
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

    public function saveHtml() 
    {
        $filename = $this->articlepath . '/' . $this->id . '.html';
        
    }

    public function saveHtmlWithComments($text) 
    {
        $filename = $this->articlepath . '/' . $this->id . '_comments.html';
    }

    public function saveHtmlPhp($text) 
    {
        $filename = $this->articlepath . '/' . $this->id . '.php';
    }

    public function saveHtmlPhpWithComments($text) 
    {
        $filename = $this->articlepath . '/' . $this->id . '_comments.php';
    }

    public function saveJson($text) 
    {
        $filename = $this->articlepath . '/' . $this->id . '.json';
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


        # strip the interactive parts from the html article
        $aa = preg_replace("/<!-- INTERACTIVE -->.*?<!-- \/INTERACTIVE -->/ms", '', $this->html_article);
        $aac = preg_replace("/<!-- INTERACTIVE -->.*?<!-- \/INTERACTIVE -->/ms", '', $this->html_comments);
        $title = htmlspecialchars($this->article['heading']);
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
}


