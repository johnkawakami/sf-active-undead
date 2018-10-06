<?php  // vim:ai:et:sw=4:ts=4:st=4:

namespace LA;

include_once(SF_SHARED_PATH."/classes/image_class.inc");
include_once(SF_SHARED_PATH."/classes/image2_class.inc");
include_once(SF_SHARED_PATH."/classes/json_article.inc");
define('COMMENT_CACHE_LIMIT', 2048);

use Nofollow;


class ArticleViews 
{
    function __construct(ArticleModel $article) 
    {
        $this->article = $article;
        $this->cachedArray = false;
    } 

    function asArray()
    {
        if ($this->cachedArray) return $this->cachedArray;

        $this->cachedArray =[
            "article" => $this->pluckArticle($article->article),
            "attachments" => $this->pluckComments($article->attachments),
            "comments" => $this->pluckComments($article->comments))
        ];
        return $this->cachedArray;
    }
    function pluckArticle(array $a) returns array 
    {
        $out = array();
        $fields = array( 'id', 'heading', 'author', 'created', 'modified',
            'last_modified', 'article', 'contact', 'link', 'address',
            'phone', 'parent_id', 'mime_type', 'summary', 'numcomment',
            'fileurl', 'filesize', 'article_url');
        foreach($fields as $field) {
            $out[$field] = $a[$field];
        }
        if (preg_match('/^image\\//',$a['mime_type'])) {
            $out['image'] = $this->findThumbs($a['fileurl']);
        }
        return $out;
    }
    function pluckComments(array $c) returns array
    {
        $out = array();
        $fields = array( 'id', 'heading', 'article', 'author', 'parent_id',
            'mime_type', 'created', 'modified', 'linked_file');
        foreach($a as $element) {
            $comment = array();
            foreach($fields as $field) {
                $comment[$field] = $element[$field];
            }
            $comment['fileurl'] = str_replace(SF_UPLOAD_PATH, 'http://la.indymedia.org/uploads', $comment['linked_file']);
            if (preg_match('/^image\\//',$comment['mime_type'])) {
                $comment['image'] = $this->findThumbs($comment['fileurl']);
            }
            unset($comment['linked_file']);
            $out[] = $comment;
        }
        return $out;
    }

    private function findThumbs( string $url ) returns array
    {
        $path = SF_UPLOAD_PATH.str_replace('http://la.indymedia.org/uploads','',$url);
        $thumbs = array('original'=>$url);
        if (file_exists($path.'thumb.jpg')) {
            $thumbs['small'] = $url.'thumb.jpg';
        }
        if (file_exists($path.'mid.jpg')) {
            $thumbs['medium'] = $url.'mid.jpg';
        }
        return $thumbs;
    }

    function json() 
    {
        return json_encode($this->asArray());
    }

    function html()
    {
    }

    function htmlWithComments($article)
    {
    }

    function phpHtml($article)
    {
    }

    function phpHtmlWithComments($article)
    {
    }

    function article()
    {
        $me = $this->asArray();
        $art = $me['article'];

        $tr = new \Translate;
        include_once(SF_CACHE_PATH.'/language_options.inc');

        $url_author = urlencode($art['author']);

        $urllink = $this->prependHttp(chop($art['link']);

        $croplink = preg_replace("/^http:\/\//i", "", $urllink);
        if (strlen($croplink) > 60) $croplink = substr($croplink, 0, 57) . "...";

        // tweak the article text based on the mime of the article
        if ($art["artmime"] == "h") {
            $scratch = Nofollow::forceNofollow($this->cleanup_html($art["article"]));
        } else {
            $scratch = $this->link_text($this->cleanup_text($this->article["article"]));
        }
        // fixme, this is a security risk
        $scratch .= $art['uploaded_article'];

        // Assign the template variables
        $defaults=(array(   'H_EADING'  =>   $art["heading"],
                            'A_UTHOR'   =>   $art["author"],
                            'U_RLAUTH'  =>   $url_author,
                            'C_DATE'    =>   $art["format_created"],
                            'A_DDRESS'  =>   $art["address"],
                            'P_HONE'    =>   $art["phone"],
                            'M_EDIA'    =>   $art["media"],
                            'S_UMMARY'  =>   $art["summary"],
                            'C_ONTACT'  =>   $art["contact"],
                            'L_INK'     =>   $urllink,
                            'C_ROPURL'  =>   $croplink,
                            'A_RTID'    =>   $art["id"],
                            'A_RTICLE'  =>   $scratch,
                            'R_ATING'   =>   floor($art["rating"]),
                            'V_ALIDATED' =>  "",
                            'L_ANGUAGE' =>   $language_options[$art['language_id']],
                            'C_ANCHOR'  =>   $art["id"],
                            'TPL_PAGE_TITLE' => urlencode($art["heading"]),
                            'TPL_SHORT_URL' => "/display.php?id=".$art["id"],
                            'N_FOLLOW'  =>   'nofollow'
                            ));

        // comments and attachments get the article id set to the parent id
        if ($art["parent_id"] != 0) { 
            $defaults['A_RTID'] = $art["parent_id"]; 
            $GLOBALS["numcomment_parent"] = $art['numcomment'];
        } 

        $this->html_article = $result_html;

        if ($art['display'] == 'f') {
            return $this->articleHidden($me);
        } else if ($art['display'] == 'l') {
            return $this->articleLocal($me);
        } else {
            return $this->articleNotHidden($me);
        }
    }
    function generateMimeThumb()
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
    function articleHidden($defaults)
    {
        $template = new \FastTemplate(SF_TEMPLATE_PATH);
        $template->clear_all();
        $template->define(array('article_page' => 'hidden.tpl'));

        $this->article['link'] = preg_replace('/\\./', '. ', $this->article['link']); 
        $this->article["article"]=$this->damage_links($this->article["article"]);
        $this->article['link']=base64_encode($this->article['link']);
        $croplink = base64_encode($croplink);

        // Process the template
        $template_obj->assign($defaults);
        $template_obj->parse('CONTENT',"article_page");
        return $template_obj->fetch("CONTENT");
    }
    function articleLocal($defaults)
    {
        $template = new \FastTemplate(SF_TEMPLATE_PATH);
        $template->clear_all();
        $template->define(['article_page' => 'text-html.tpl']);

        $defaults['A_RTICLE'] = Nofollow::removeNofollow($defaults['A_RTICLE']);
        $defaults['N_FOLLOW'] = 'dofollow'; 

        // Process the template
        $template_obj->assign($defaults);
        $template_obj->parse('CONTENT',"article_page");
        return $template_obj->fetch("CONTENT");
    }
    function articleNotHidden($defaults)
    {
        $template = new \FastTemplate(SF_TEMPLATE_PATH);
        $template->clear_all();
        $template->define(['article_page' => 'text-html.tpl']);

        // Process the template
        $template_obj->assign($defaults);
        $template_obj->parse('CONTENT',"article_page");
        return $template_obj->fetch("CONTENT");
    }

    static function commentsAsList($comments)
    {
    }

    static function comments($comments)
    {
    }

    static function attachments($attachments)
    {
    }

    private function prependHttp($urllink) 
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

        $urllink = $this->prependHttp($urllink);

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
}


