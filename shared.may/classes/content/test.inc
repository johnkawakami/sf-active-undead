<?php

include_once ('shared/classes/content_class.inc');

class Test extends Content 
{
    // New Content Class - will make stuff like the page class, but with a new template system
	// it only gives you
	function Test () {
		//
	}
	
    function Execute () {		
		return "Hello World";
    }

    
} // end class

?>
