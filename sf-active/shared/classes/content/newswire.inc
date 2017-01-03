<?php

include_once (SF_CLASS_PATH.'/content_class.inc');

class Newswire extends Content 
{
    // New Content Class - will make stuff like the page class, but with a new template system
	// it only gives you
	function Newswire () {
		//
	}
	
    function Execute () {	

		$data = $this->getNewswireList(" LIMIT 30");
		$cache = new Cache();
		
		// Parsing data, making i18n date, etc...
		foreach ($data as $key => $val) {
			$data[$key]['case'] = $this->getTypeforCase ($data[$key]['case']);
		    $link = $cache->render_yearmonth_link( $data[$key]['created_year'], $data[$key]['created_month']);
			$data[$key]['link'] = "/news/" . $link . $data[$key]['id'] . ".php";
//			$data[$key]['created'] = strftime ("%x", ($data[$key]['case']));            
			$data[$key]['created'] = $cache->render_protest_date ( $data[$key]['created'] );
            $data[$key]['heading'] = $cache->render_entities ( $data[$key]['heading'] );
		}
		
		return $data;
    }
	
	
    function getNewswireList ($where) {
    // Returns a resultset of articles for a newswire summary list

    	$db = new DB();
		
		// sql query - no date calculation stuff to keep mysql load down
		// case is need for the template system
 		
        global $time_diff;
		$op="+";
		if ($time_diff<0){
			$ltimediff=$time_diff*-1;
			$op="-";
		}
		else{
			$ltimediff=$time_diff;
		}

        $query = "SELECT w.id AS id, 
						 w.heading AS heading, 
						 w.mime_type AS mime_type,
						 w.mime_type AS 'case',
                         date_format(w.created, '%Y') AS created_year,
                         date_format((w.created $op interval $ltimediff second), '%b %d %l:%i%p') AS created,
                         date_format(w.created, '%m') AS created_month,
                         date_add(w.created, interval 48 hour) AS plus_twelve, 
						 now() AS now
						 FROM webcast w".$where;
/*
        if ($cat > 0) {
			$query .= ",catlink l";
        	$query .= " WHERE w.display='" . $post_status . "' AND ";
		}

        if ($cat > 0) {
			$query .= "l.id = w.id AND l.catid=$cat AND ";
	        $query .= "w.parent_id=0 ORDER BY w.id DESC LIMIT $length";
		}
*/
        $resultset = $db->query($query);
        return $resultset;
    }
	
	function getCases () {
		return array('audio', 'video', 'image', "pdf");
	}
	
	function getTypeforCase ($val) {
		
		if ( strpos ($val, "image") !== FALSE )
			$ret = "image";
			
		if ( strpos ($val, "shockwave") !== FALSE )
			$ret = "image";
		
		if ( strpos ($val, "audio") !== FALSE )
			$ret = "audio";
		
		if ( strpos ($val, "smil") !== FALSE )
			$ret = "video";	

		if ( strpos ($val, "video") !== FALSE )
			$ret = "video";	

		if ( strpos ($val, "pdf") !== FALSE )
			$ret = "pdf";	
			
		return $ret;
	}
   
} // end class

?>
