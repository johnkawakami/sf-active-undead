<?php

// Class for file_viewer page

class file_viewer extends Page {

    function file_viewer() {
        return 1;
    }

    function execute() {
	//This page displays the main list of admin options
	$str1=$_GET["str1"];
	if (strlen($str1)<1){
		$str1=$_POST["str1"];
	}
	$str2=$_GET["str2"];
	if (strlen($str2)<1){
		$str2=$_POST["str2"];
	}
	if (strlen($str1)<1){
		$str1="";
	}
	if (strlen($str2)<1){
		$str2="";
	}
	$filename=$_GET["filename"];
	if (strlen($filename)<1){
		$filename=$_POST["filename"];
	}
	$fullfilename=SF_CACHE_PATH."/".$filename;
	
	if (strlen($str1)<1 && strlen($str1)<1 &&strlen($_POST["save"])>0){
		$lf=fopen($fullfilename,"w");
		$write=stripslashes($_POST[contents]);
		//echo $write;
		fwrite($lf, $write);
		fclose($lf);
	}	

	$lf=fopen($fullfilename,"r");
	if ($lf){
		$fsize=filesize($fullfilename);
		while (!feof ($lf)) {
    			$nextline=fgets($lf,400);
    			if (strlen($str1)<1 || strpos($nextline, $str1)>-1){
				if (strlen($str2)<1 || strpos($nextline, $str2)>-1){
    					$contents.= $nextline;
    				}
			}
		}
		fclose($lf);
		$this->tkeys['contents'] = $contents;
        	$this->tkeys['filename'] = $filename;
        	$this->tkeys['filter1'] = $str1;
        	$this->tkeys['filter2'] = $str2;
        	$this->tkeys['filter_size'] = $backlog;
		return 1;
		fopen($lf);
	}
    }

}

?>


