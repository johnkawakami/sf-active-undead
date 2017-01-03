<?
$chmod_dir = "/www/uploads/melbourne";

function file_dir($idir) {

   // this function returns directory listing of certain filetypes we may
need

  	$dh=dir($idir);
	$basename = basename($dh->path);
	$dirlist = array();

	while ($file_name = $dh->read()){
	
		if(substr($file_name,0,1)==".") continue;

		$file_path = $dh->path . "/" . $file_name; 
		
		if(!is_dir($file_path)) {

			if(preg_match("(jpg|jpeg|gif|png|txt|pdf|htm)",$file_name)) $dirlist[] = $file_path;			
				
                 }
    	
  	}
	  
  return $dirlist;
  
}
  $find_ar = file_dir($chmod_dir);	
  echo "here";
  for($i=0;$find_ar[$i];$i++) {

	  echo "<br>{$find_ar[$i]}";
          chmod ($find_ar[$i],0644);

  }
?>
