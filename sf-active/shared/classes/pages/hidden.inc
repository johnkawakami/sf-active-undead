<?php

// Class for displaying hidden articles

class hidden extends Page
{

    function newswire()
    {
        return 1;
    }

    function execute()
    {

include_once(SF_CLASS_PATH.'/category_class.inc');

$id=$_GET['id'];

$article_obj = new Article($id);

$article_obj->render_everything();

$result = $article_obj->html_article . $article_obj->html_attachments . $article_obj->html_comments;

$this->tkeys['result']=$result;

}

}

?>
