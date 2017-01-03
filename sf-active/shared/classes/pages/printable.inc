<?php

// Class for the Calender Add and Update Form

class printable extends Page {

    function printable () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
	
		
		// Translate all TPL_ in the Template
		$tr = new Translate();
		$tr->create_translate_table('printable');
		$this->translation($tr);

		if ( $_GET['id'] > 0 ) 
		{

			$article = new Article ($_GET['id']);
			$article->render();
			$article->set_paths();

			$this->tkeys['PRN_ARTICLE_HTML'] = $article->html_article;
			$this->tkeys['PRN_ARTICLE_URL'] = $article->article['article_url'];
			$this->tkeys['PRN_TITLE'] = $article->article['heading']." : ".$GLOBALS['site_nick'];

			//$PHP_SELF."?id=".$_GET['id'];	
			//$PHP_SELF."?id=".$_GET['id']."&comments=yes";

			if ( $_GET['comments'] == "yes" ) 
			{
				
				$this->tkeys ['PRN_COMMENT_LINK'] = $PHPSELF."?id=".$_GET['id'];
				$this->tkeys ['PRN_COMMENT_TEXT'] = $tr->trans('prn_hide_comments');
				
				if ( $article->article['numcomment'] > 0 ) 
				{
					$article->render_all_comments();
					$this->tkeys ['PRN_COMMENTS_HTML'] = $article->html_comments;
					
				} else {
				
					$this->tkeys ['PRN_COMMENTS_HTML'] = $tr->trans('prn_no_comments');
					
				}
	
			} else {
	
				$this->tkeys ['PRN_COMMENT_LINK'] = $PHPSELF."?id=".$_GET['id']."&comments=yes";
				$this->tkeys ['PRN_COMMENT_TEXT'] = $tr->trans('prn_print_comments');
				$this->tkeys ['PRN_COMMENTS_HTML'] = "";
				
	
			}

			
		} else {
			$this->tkeys ['PRN_COMMENTS_HTML'] = "";
			$this->tkeys['PRN_TITLE'] = "";
			$this->tkeys['PRN_ARTICLE_URL'] = "";
			$this->tkeys ['PRN_COMMENT_LINK'] = "";
			$this->tkeys ['PRN_COMMENT_TEXT'] = "";
			$this->tkeys ['PRN_ARTICLE_HTML'] = $tr->trans('prn_no_id');
		}
		
		return 1;		
	}

	function translation ($tr) {
		$this->tkeys['LANG_PRN_ORGINAL_ARTICLE'] = $tr->trans('prn_orginal_article');
		$this->tkeys['LANG_PRN_SITE_NAME'] = $GLOBALS['site_name'];
	}

}
