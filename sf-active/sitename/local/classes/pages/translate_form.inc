<?php
// Class for Left Center

class translate_form extends LocalPage {

    function translate_form () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
		//
		$this->tkeys['LANG_TRADUCIR'] = $GLOBALS['dict']['traducir'];
		$this->tkeys['URL'] = 'http://'.$_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI'];
	}
	
}

?>





