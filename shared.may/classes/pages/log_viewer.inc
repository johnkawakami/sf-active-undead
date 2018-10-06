<?php

// Class for log_viewer page

class log_viewer extends Page {

    function log_viewer() {
        return 1;
    }

    function execute() {
	//This page displays the main list of admin options
	$str1=$_POST["str1"];
	$str2=$_POST["str2"];
	$backlog=$_POST["backlog"];
	$logfilename="/u2/indy/admin/ip_access_log";
	$lf=fopen($logfilename,"r");
	if ($lf){
		$fsize=filesize($logfilename);
		//echo "file size was:".$fsize."<br>";
		if (sizeof($backlog)==0){
			$backlog=5000000;
		}
		if (sizeof($str1)==0){
			$str1=POST;
		}
		if (sizeof($str2)==0){
			$str2=publish;
		}
		if ($fsize>$backlog){
			fseek($lf,$fsize-$backlog);
		}
		while (!feof ($lf)) {
    			$nextline=fgets($lf,400);
    			if (strpos($nextline, $str1)>-1){
				if (strpos($nextline, $str2)>-1){
    					$contents = $nextline.$contents;
    				}
			}
		}
		fclose($lf);
		$this->tkeys['contents'] = $contents;
        	$this->tkeys['logfile_name'] = $logfilename;
        	$this->tkeys['filter1'] = $str1;
        	$this->tkeys['filter2'] = $str2;
        	$this->tkeys['filter_size'] = $backlog;
		return 1;
	}
    }

}

?>


