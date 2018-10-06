<?php  // vim:et:sw=4:ts=4:st=4:ai

namespace \LA\Traits;

/**
 * Helper to save files related to ArticleModel.
 *
 * Establishes paths, canonical URLs, url links to files.
 */
class UploadFiles 
{
    private $article;
    private $id;
    private $filebasename;
    private $filepath;
    private $fileurl;

    function __construct(Article $article)
    {
        $this->article = $article;

        $linked_file = $article->article['linked_file'];
        $file_exists = file_exists($linked_file);
        $year = $article->article["created_year"];
        $month = $article->article["created_month"];

        if ($file_exists) {
            $this->basename = basename($linked_file);
            if(preg_match("/$year\/$month/", $linked_file))
            {
                $this->filepath = $linked_file;
                $this->fileurl  = $this->filePathToUrl( $linked_file );
            }
            else
            {
                // legacy uploads
                $this->filepath = SF_UPLOAD_PATH . "/" . $this->basename;
                $this->fileurl  = SF_UPLOAD_URL  . "/" . $this->basename;
            }
            // fixme - move this out of here
            $this->render_file_size();
        }
    }

    function getFilePath()
    {
        return $this->filepath;
    }

    function getFileUrl() 
    {
        return $this->fileurl;
    }

    function getCanonicalFilePath($file)
    {
        $basename = basename($file);
        return SF_UPLOAD_PATH . "/$year/$month/$basename";
    }

    function filePathToUrl($path)
    {
        return str_replace( SF_UPLOAD_PATH, SF_UPLOAD_URL, $path );
    }

    /**
     * Move the attached file to its canonical location in uploads/yyyy/mm/
     * Old sf-active uploads all went into uploacs/, and this can fix it.
     */
    function moveToCanonicalFilePath()
    {
        $attachment->handle_move_uploads();
        $attachment->set_paths();
        $attachment->set_mime_type();
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

    function regeneratePreviews( array $article )
    {
        $mimeview = MimeViewFactory::create( $article['mime_type'] );
        $mimeview->load( $article['linked_file'] );
        $mimeview->setPath( $this->filepath );
        $mimeview->createThumbnails();
        return $mimeview->getHtmlPreview();
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


    function moveUploads()
    {
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


