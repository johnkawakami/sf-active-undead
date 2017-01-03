<?php

include_once (SF_CLASS_PATH.'/btemplate_class.inc');

class Content 
{
    // New Content Class - will make stuff like the page class, but without a template
	// only gives you data, the producder starts the template and looks wich content is needed
	
	
    function Content ($id='') {

		include_once (SF_CLASS_PATH.'/content/'.$id.'.inc');
		
//	 	if (!class_exists($id) ) {
           	$this->content = new $id();
//        }
		
    }
	
	// this gives you back what the btemplate is awaiting... string, array, case array or whatever..
	function getContent () {
		return $this->content->Execute();
	}
    
	function getCases () {
		return $this->content->getCases();
	}
	
} // end class

?>
