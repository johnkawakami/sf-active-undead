<?php

// Class for article_regenerate

class article_regenerate extends Page
{

    function article_regenerate ()
    {
        return 1;
    }

    function execute()
    {
	
        $tblhtml = "";
        $this->tkeys['local_result_html'] = '';
        include_once(SF_SHARED_PATH . '/classes/category_class.inc');
        include_once(SF_SHARED_PATH . '/classes/feature_class.inc');
        include_once(SF_SHARED_PATH . '/classes/date_class.inc');

        $include_form = 1;
        $category_selector = 1;
        $edit = 1;

        if ($_POST['regenerate'] != "")
        {
            $tr=new Translate();
            $tr->create_translate_table('article_regenerate');
	
            $i = 0;
            $article_obj = new \SFACTIVE\Article();
            $article_list = $article_obj->get_post_ids_starting_with($_POST['start_id'], $_POST['max_to_gen']);
            $result_html="<br />";
            while (($next_row = array_pop($article_list)))
           {
		// fixme - code stink. this is the only place where we set this
		// this function should drill into the comments and attachments
		// and perform the file moving stuff.
		/*  Thsi is what it does
                    $comment->handle_move_uploads();
                    $comment->set_paths();
                    $comment->set_mime_type();
		*/

                $GLOBALS['move_attachments'] = '1';
                $id = $next_row['id'];
                $article = new \SFACTIVE\Article($id);
                $article->handle_move_uploads();
                $article->render_everything();
                $article->cache_to_disk();
                $result_html .= "<br />";
                $result_html .= $tr->trans('id_article').": $id<br />";
                //$result_html .= $tr->trans('wrote_to').": $filename<br />";
            }
            $this->tkeys['local_result_html'] = $result_html;
        }

        if (($_POST['start_id']!="") && ($id!=""))
        {
            $nextid = $id+1;
	    $this->tkeys["value_start_id"]=$nextid;
	    $this->tkeys["value_max_to_gen"]=$_POST['max_to_gen'];
	}else{
	    $this->tkeys["value_start_id"]='';
	    $this->tkeys["value_max_to_gen"]='';
	}	
	    

        return 1;
    }
}

