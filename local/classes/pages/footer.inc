<?php
// Class for Disclaimer

class footer extends LocalPage {

    function footer () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
	
		// Left No Left
		if ($GLOBALS['left'] == "noleft") 
		{ 		
			$this->tkeys['LEFT'] = '<table class="bgult" cellpadding="0" cellspacing="0" border="0">'.
								   '<tr><td><table cellpadding="0" cellspacing="1" border="0">'; 
		} else { 
			// dont know if that is 100% right - FIXME ? 
			$this->tkeys['LEFT'] = '</td></tr></table></td></tr></table></td></tr></table>';
		}

		// Disclaimer
		$localpage = new LocalPage('disclaimer');

		if ($localpage->get_error())
		{
    		$this->tkeys['DISCLAIMER'] =  "Fatal error: " . $localpage->get_error();
		} else
		{
    		$localpage->build_page();
    		$this->tkeys['DISCLAIMER'] =  $localpage->get_html();
		}
		
		// IMC List
		
		$filename = SF_SHARED_PATH."/cities.inc";
		
		if ( file_exists( $filename ) )
		{				

			$fcach=fopen($filename, "r");
			$fcachcontent = fread( $fcach, filesize($filename) );
			fclose($fcach);
				
			$this->tkeys['IMC_LIST'] = $fcachcontent;

		} else {

			$this->tkeys['IMC_LIST'] = $fcachcontent;
		}

	}
	
}

?>





