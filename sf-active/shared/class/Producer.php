<?php

include ("shared/global.cfg");
include_once (SF_CLASS_PATH.'/content_class.inc');
include_once (SF_CLASS_PATH.'/btemplate_class.inc');

class Producer 
{
    function Producer () {
		$tpl = new bTemplate();
		$nw = new Content('newswire');
		$tr = new Translate;

		// simpel style without "if" - $tpl->set('newswire', $nw->getContent() );   
		$tpl->set_cloop('newswire', $nw->getContent() , $nw->getCases() );

		// translation
		$tpl->set('lang', array('newswire' => $GLOBALS['dict']['newswire'] ) );

		$html = $tpl->fetch(SF_TEMPLATE_PATH.'/index.template');
    }
} // end class

?>
