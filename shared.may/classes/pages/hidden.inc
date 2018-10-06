<?php

// Class for displaying hidden articles
include_once(SF_CLASS_PATH.'/category_class.inc');

class hidden extends Page
{
    function hidden() {
	$id=intval($_GET['id']);
	$this->article_obj = new \SFACTIVE\Article($id);

	// if the article is not hidden, and the children are not hidden, then redirect to the article
	if (false and $this->article_obj->article['display']!='f') {
		header("Location: /display.php?id=".$id, TRUE, 301);
	}
    }

    function newswire()
    {
        return 1;
    }

    function execute()
    {
	$this->article_obj->render_everything();
	$result = $this->article_obj->html_article . $this->article_obj->html_attachments . $this->article_obj->html_comments;
	$this->tkeys['result']=$result;
    }

}

?>
