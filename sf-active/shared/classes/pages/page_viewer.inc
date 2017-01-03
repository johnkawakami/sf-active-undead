<?php

// Class for page_viewer page

class page_viewer extends Page {

    function page_viewer() {
        return 1;
    }

    function page_validate_name ($filename) {
	if ( (eregi("^[_0-9a-z-]+",$filename)) && (eregi ("(.)+\\.inc$",$filename)) ) return 1;
	else return 0;
    }

    function execute() {
	//This page displays the main list of admin option
	
	if ((!isset($_GET[filename])) && (!isset($_REQUEST[filename]))) { 
	$this->tkeys[insert_filename]="<b>TPL_INSERTNAME</b>:  <input type=\"text\" name=\"filename\"><br><input type=\"hidden\" name=\"addpage\" value=\"1\">";
	$this->tkeys[filter1]="";
	$this->tkeys[filter2]="";
	}elseif ( (!$this->page_validate_name ($_REQUEST[filename])) && ($_REQUEST[addpage]=="1") ) { $this->tkeys[filename_header]="TPL_ERRORpage";
	$this->tkeys[insert_filename]="<b>TPL_INSERTNAME</b>:  <input type=\"text\" name=\"filename\"><br><input type=\"hidden\" name=\"addpage\" value=\"1\">";
	$this->tkeys[filter1]="";
	$this->tkeys[filter2]="";
	} else {
	$this->tkeys[filename_header]="TPL_FILENAME";
	$this->tkeys[insert_filename]="<input type=\"hidden\" name=\"filename\" value=\"TPL_FILENAME\">";
	}
	
	$str1=$_GET["str1"];
	if (strlen($str1)<1){
		$str1=$_REQUEST["str1"];
	}
	$str2=$_GET["str2"];
	if (strlen($str2)<1){
		$str2=$_REQUEST["str2"];
	}
	if (strlen($str1)<1){
		$str1="";
	}
	if (strlen($str2)<1){
		$str2="";
	}
	$filename=$_GET["filename"];
	if (strlen($filename)<1){
		$filename=$_REQUEST["filename"];
	}
	// this makes sure nothing is created if not with a valid page name
 	if ($this->page_validate_name($filename)) {	
	$fullfilename=SF_CACHE_PATH."/pages/".$filename;
	}
	else $fullfilename="";

	
	if (strlen($str1)<1 && strlen($str1)<1 && strlen($_REQUEST["save"])>0){

		if (!is_file($fullfilename))  {
                $lf=fopen($fullfilename,"a");
		}elseif ((is_file($fullfilename)) && ($_REQUEST[addpage]=="1")){
		$lf=fopen($fullfilename,"r");
		}else{ 
		$lf=fopen($fullfilename,"w");
		}
		$write=stripslashes($_REQUEST[contents]);
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
		$this->create_process_page($filename);
		$this->tkeys['contents'] = $contents;
        	$this->tkeys['filename'] = $filename;
        	$this->tkeys['filter1'] = $str1;
        	$this->tkeys['filter2'] = $str2;
        	$this->tkeys['filter_size'] = $backlog;
		return 1;
		fopen($lf);
	}
    }

	function create_process_page($filename)
	{
		list($new_file, $junk) = explode(".", $filename, 2);
		if(!file_exists(SF_WEB_PATH."/process/".$new_file.".php"))
		{
			$file = "<?php\n";
			$file .= "// ".$new_file." content page\n\n";
			$file .= "include_once(\"shared/global.cfg\");\n\n";
			$file .= "sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');\n\n";
			$file .= "\$page = new Page('content_page');\n\n";
			$file .= "if (\$page->get_error()) {\n";
			$file .= "    echo \"Fatal error: \" . \$page->get_error();\n";
			$file .= "} else {\n";
			$file .= "    \$page->build_page('".$new_file."');\n";
			$file .= "    echo \$page->get_html();\n";
			$file .= "}\n\n";
			$file .= "sf_include_file(SF_INCLUDE_PATH, 'content-footer.inc');\n\n";
			$file .= "?>";

			$this->cache_file($file, SF_WEB_PATH."/process/".$new_file.".php");
		}
	}

}

?>
