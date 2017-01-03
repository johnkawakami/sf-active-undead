<?php
// Class for Left Center

class content_left extends LocalPage {

    function content_left () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
	
		// Left No Left
		if ($GLOBALS['left'] == "noleft") 
		{ 		
			$this->tkeys['LEFT'] = '';
								   
		} else { 

			$filename = $GLOBALS['left'];
		
			if ( file_exists( $filename ) )
			{				

				$fcach=fopen($filename, "r");
				$fcachcontent = fread( $fcach, filesize($filename) );
				fclose($fcach);
				$this->tkeys['LEFT'] = $fcachcontent;

			} else {

				$this->tkeys['LEFT'] = '';
			}
		}

		// Search
		$localpage = new LocalPage('search');

		if ($localpage->get_error())
		{
    		$this->tkeys['SEARCH'] =  "Fatal error: " . $localpage->get_error();
		} else
		{
    		$localpage->build_page();
    		$this->tkeys['SEARCH'] =  $localpage->get_html();
		}

		// Search
		$localpage = new LocalPage('translate_form');

		if ($localpage->get_error())
		{
    		$this->tkeys['TRANSLATE_FORM'] =  "Fatal error: " . $localpage->get_error();
		} else
		{
    		$localpage->build_page();
    		$this->tkeys['TRANSLATE_FORM'] =  $localpage->get_html();
		}
		

	}
	
}

?>
