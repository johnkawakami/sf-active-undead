<?php

// Class for include_viewer page

class include_viewer extends Page {

    function include_viewer() {
        return 1;
    }

    function include_validate_name ($filename) {
	if ( (eregi("^[_0-9a-z-]+",$filename)) && (eregi ("(.)+\\.(inc$|php$)",$filename)) ) return 1;
	else return 0;
    }


    function execute() {
	//This page displays the main list of admin options
	if ((!isset($_GET[filename])) && (!isset($_POST[filename]))) { 
	$this->tkeys[insert_filename]="<b>TPL_INSERTNAME</b>:  <input type=\"text\" name=\"filename\"><br><input type=\"hidden\" name=\"addinclude\" value=\"1\">";
	$this->tkeys[filter1]="";
	$this->tkeys[filter2]="";
	}elseif ( (!$this->include_validate_name ($_POST[filename])) && ($_POST[addinclude]=="1") ) { $this->tkeys[filename_header]="TPL_ERRORINC";
	$this->tkeys[insert_filename]="<b>TPL_INSERTNAME</b>:  <input type=\"text\" name=\"filename\"><br><input type=\"hidden\" name=\"addinclude\" value=\"1\">";
	$this->tkeys[filter1]="";
	$this->tkeys[filter2]="";
	} else {
	$this->tkeys[filename_header]="TPL_FILENAME";
	$this->tkeys[insert_filename]="<input type=\"hidden\" name=\"filename\" value=\"TPL_FILENAME\">";
	}
	
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
	// this makes sure nothing is created if not with a valid css name
 	if ($this->include_validate_name($filename)) {	
	$fullfilename=SF_INCLUDE_PATH."/".$filename;
	}
	else $fullfilename="";
	
	if (strlen($str1)<1 && strlen($str1)<1 && strlen($_POST["save"])>0){

		if (!is_file($fullfilename))  {
                $lf=fopen($fullfilename,"a");
		}elseif ((is_file($fullfilename)) && ($_POST[addinclude]=="1")){
		$lf=fopen($fullfilename,"r");
		}else{ 
		$lf=fopen($fullfilename,"w");
		}
		$write=stripslashes($_POST[contents]);
		//echo $write;
		fwrite($lf, $write);
		fclose($lf);
	}	
	if (!is_file($fullfilename)) {
		$lf=fopen($fullfilename,"a");
	}else{
	$lf=fopen($fullfilename,"r");
	}
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


