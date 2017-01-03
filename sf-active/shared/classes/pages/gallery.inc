<?php

class gallery extends page 
{

    function gallery() 
    {
	return 1 ;
    }

    function execute()
    {
	if($_GET)
	{
	    $medium = $_GET['medium'] ;
	    if($_GET['month'])
	    {
		$month = $_GET['month'] ;
		if($month < 10) $month = "0$month";
	    }else{
		$month = date('m');
	    }
	    if($_GET['year']) 
	    { 
		$year = $_GET['year']; 
	    }else{
		$year = date('Y');
	    }	    
	    $category_id = $_GET['category_id'] ;
	}else{
	    $medium = 'image';
	    $category_id = '';
	    $year = date('Y');
	    $month = date('m');
	}
	$new_location = SF_UPLOAD_URL.'/'.$year.'/'.$month.'/?category_id='.$category_id.'&medium='.$medium ;
	header("Location: $new_location");
    }
}
?>
