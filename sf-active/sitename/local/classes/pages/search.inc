<?php
// Class for Left Center

class search extends LocalPage {

    function search () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
	
	
		if ($_GET['medium'] =="text") 
		{
			$this->tkeys['TEXT_SEL'] = 'selected';
		} else {
			$this->tkeys['TEXT_SEL'] = '';
		}
		
		if ($_GET['medium'] =="image") 
		{
			$this->tkeys['IMAGE_SEL'] = 'selected';
		} else {
			$this->tkeys['IMAGE_SEL'] = '';
		}

		if ($_GET['medium'] =="audio") 
		{
			$this->tkeys['AUDIO_SEL'] = 'selected';
		} else {
			$this->tkeys['AUDIO_SEL'] = '';
		}
		
		if ($_GET['medium'] =="video") 
		{
			$this->tkeys['VIDEO_SEL'] = 'selected';
		} else {
			$this->tkeys['VIDEO_SEL'] = '';
		}

		if ($_GET['medium'] =="application") 
		{
			$this->tkeys['APPLICATION_SEL'] = 'selected';
		} else {
			$this->tkeys['APPLICATION_SEL'] = '';
		}
		
		$this->tkeys['LANG_SEARCH'] = $GLOBALS['dict']['search'];
		$this->tkeys['LANG_MEDIUM'] = $GLOBALS['dict']['medium'];
		$this->tkeys['LANG_TEXT'] = $GLOBALS['dict']['text'];
		$this->tkeys['LANG_IMAGE'] = $GLOBALS['dict']['image'];
		$this->tkeys['LANG_VIDEO'] = $GLOBALS['dict']['video'];
		$this->tkeys['LANG_AUDIO'] = $GLOBALS['dict']['audio'];
		$this->tkeys['LANG_OTHER'] = $GLOBALS['dict']['other'];

	}
	
}

?>





