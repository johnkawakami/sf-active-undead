<?php
//NONCLASSFILE IN CLASSES FOLDER
//This file contains code to display a list of images
//it should be converted to a class

class image_browser extends Page
{

	function image_browser()
	{
		//does nothing, is a class constructor
		return 1;
	}

	function execute()
	{

		// setting up variables.
		$title='';                          // title of table, leave blank to use default
	        $showfilenames=0;                   // showfilenames .. if disabled and file has no description, name will be used.
	        $openwindow=0;                      // open in popup, 1 = true, 0 = false
	        $showimagesize=1;                   // show image height/width
	        $thumbnails=0;                      // show image thumbnails
	        $browse=1;                          // allow subdirectory browsing
	        $fadethumbs=1;                      // apply fade effect to thumbnail mouseovers

	        $os="NIX";                          // Operating System, set as "WIN" if you are using Windows, otherwise leave as "NIX"
		                                    // Subdirectory browsing is disabled on windows machines.

																	
		$imconvert = $GLOBALS['imconvert_path'];
		$rootdir = '.';
		$filetypes='(gif|jpeg|jpg|png|swf)';

		// modify look/layout
		 $tdcolor="#f1f1f1";        	// main td color 
		 $tdovercolor="#fffff1";    	// td mouseover color 
		 $bordercolor="#000000";     	// table border color 
		 $textcolor="#000000";       	// td text color 
		 $tablewidth='500';				// width of the table.. you might want to change this to fit your layout better.
 

 ////////////////////////
 // SCRIPT BEGINS HERE 
 
 
 
 if ($x == "view") { 
 	echo '<html><head><title>Viewing Image</title></head><body bgcolor="'.$tdcolor.'"  topmargin="0" leftmargin="0"><div align="center">
 	<a href="javascript:void(0)" onclick="window.close()"><font face="verdana" size="1" color="'.$textcolor.'"><b>[ Close Window ]</b></a><br><br></font>
 	<img src="'.$img.'"></body></html>'; 
 	exit();
 	} 
 $showdir = ereg_replace("\.\./","",$showdir); 
 if ($showdir) { $listdir = $rootdir.ereg_replace("\./","",$showdir); } else { $listdir = $rootdir; }
 



 $handle=@opendir($listdir);
 if (!$handle) { echo "no directory ('.$listdir.'), dying."; exit(); }
 
   while (($file = readdir($handle))!==false) {
   	$filearray[count($filearray)] = $file; 
   } 
   asort($filearray); closedir($handle); 
 
  // count files
  $filecount=0;
  foreach ($filearray as $file) { 
	   $name = split("\.",$file);
   	if ((eregi($filetypes,$name[count($name)-1]) == true) && (eregi("tedlsthumb",$file) == false)) { $filecount++; }
   }
	
  // deal with title	
  if (!$title) $title = 'Listing all available images in '.$listdir; 
  $title.=' ('.$filecount.' files)';
 
 
  echo '
   <script language="JavaScript">
   function openl(img,nw,nh) {';
   if ($openwindow == 1) { echo '


   window.open("?x=view&img=" + img, nw+nh, "toolbar=no,menubar=no,location=no,scrollbars=no,resize=no,width=" + nw + ",height=" + nh);'; }
   else { echo 'document.location=img;'; }
   echo '}</script>  
<table bgcolor="'.$bordercolor.'" cellpadding="0" cellspacing="0" border="0"><tr><td>
  <table cellpadding="1" cellspacing="1" width="'.$tablewidth.'">
  <tr><td colspan="3" bgcolor="'.$tdcolor.'" align="center"><font face="verdana" size="1" color="'.$textcolor.'"><b>'.$title.'</b></font></td></tr>';
 
  // loop through all files, build and sort array

	if ($os == "WIN") { 
		$tmp = split('/',$SCRIPT_NAME);
   	$docroot = @ereg_replace($tmp[count($tmp)-1],'',$SCRIPT_NAME);
   } else { 
		 	$tmp = split('/',$SCRIPT_FILENAME);
   		$docroot = @ereg_replace($tmp[count($tmp)-1],'',$SCRIPT_FILENAME);   	 	
   	}
   @chdir($docroot.$listdir);

  // subdirectory browsing	
  if (($browse==1) && ($os != "WIN")) { 
    if ($showdir) { 
     $updir = split("/",$showdir);
     $tmp = count($updir)-1;
     $i=0;
     while ($i<$tmp) { 
      $back.=$updir[$i].'/';
      $i++;
      }
     $back = substr($back,0,strlen($back)-1);
     echo '<tr><td colspan="3" bgcolor="'.$tdcolor.'" align="center"><font face="verdana" size="1" color="'.$textcolor.'"><b><a href="'.$_SERVER['PHP_SELF'].'?showdir='.$back.'">« parent directory</a></b></font></td></tr>'; 
    } 
    foreach ($filearray as $file) {
    	
  		if (is_dir($file) && $file != '.' && $file != '..') {
  			// count files
  			$tmphandle=@opendir($file);
 			$subfilecount=0;
		   while (($tmpfile = readdir($tmphandle))!==false) {
		   	$tmpname = split("\.",$tmpfile);
   			if ((eregi($filetypes,$tmpname[count($tmpname)-1]) == true) && (eregi("tedlsthumb",$tmpfile) == false)) { 
   				$subfilecount++; }
   		}
   		closedir($tmphandle);   
     		echo '<tr><td colspan="3" bgcolor="'.$tdcolor.'" align="center"><font face="verdana" size="1" color="'.$textcolor.'"><b><a href="'.$_SERVER['PHP_SELF'].'?showdir='.$showdir.'/'.$file.'">'.$file.'</a> ('.$subfilecount.' files)</b></font></td></tr>';
     	}
    }
   }

  // loop all files, and filter file types
  foreach ($filearray as $file) { 
   $name = split("\.",$file);
     
   if (eregi($filetypes,$name[count($name)-1]) == true) {
     // get filesize and translate to human readable size
	
     $file_size = filesize($file); 
     if ($file_size >= 1073741824) { 
      $file_size = round($file_size / 1073741824 * 100) / 100 . "g"; 
     } elseif ($file_size >= 1048576) { 
      $file_size = round($file_size / 1048576 * 100) / 100 . "m"; 
     } elseif ($file_size >= 1024) { 
      $file_size = round($file_size / 1024 * 100) / 100 . "k"; 
     } else { 
      $file_size = $file_size . "b"; 
     } 
     
    unset($fileinfo);
    if (file_exists($file.'.desc')) {
    	if ($showfilenames == 1) $fileinfo = '<b>'.$file.'</b><br>';
    	$fdesc = fopen ($file.'.desc',r);
		$fileinfo .= fread ($fdesc, filesize ($file.'.desc'));
		fclose ($fdesc);
    } else { 
    	$fileinfo = '<b>'.$file.'</b> ';
    	}
	  
     if ($size = @getimagesize($file)) {
      $nw = $size[0] + 23;
      $nh = $size[1] + 40;
      }
	  if ($showimagesize == 1) { 
	  $imagesize = $size[0].'x'.$size[1];
     }
   
   if ($file != '.' && $file != '..' && !ereg('.desc',$file) && !ereg('tedlsthumb',$file)) { 
     echo '<tr>';
     // thumbnail handling
     if ($thumbnails==1) { 
     	if ($imconvert && !file_exists($name[0].'_tedlsthumb.jpg')) exec($imconvert.' -quality 70 -geometry 80X50 '.$docroot.$listdir.'/'.$file.' '.$docroot.$listdir.'/'.$name[0].'_tedlsthumb.jpg');
    	if (file_exists($name[0].'_tedlsthumb.jpg')) { 
    	  $thumbnailimg = $listdir.'/'.$name[0].'_tedlsthumb.jpg'; 
    	  } else { $thumbnailimg = $listdir.'/'.$file; }
		// fade effect toggle    
    	if ($fadethumbs == 1) { 
    		 echo '<td valign="top" bgcolor="'.$tdcolor.'" style="cursor: hand" onclick="openl(\''.$listdir.'/'.$file.'\',\''.$nw.'\',\''.$nh.'\')" onmouseover="a'.$this->cleanData($file).'.filters.alpha.opacity=\'100\';" onmouseout="a'.$this->cleanData($file).'.filters.alpha.opacity=\'75\';" width="80"><img style="filter:alpha(opacity=80);" id="a'.$this->cleanData($file).'" src="'.$thumbnailimg.'" width="80" height="50"></td>';
    		 } else { echo '<td valign="top" bgcolor="'.$tdcolor.'" style="cursor: hand" onclick="openl(\''.$listdir.'/'.$file.'\',\''.$nw.'\',\''.$nh.'\')" width="80"><img id="a'.$this->cleanData($file).'" src="'.$thumbnailimg.'" width="80" height="50"></td>'; }
    }
    $tdwidth = $tablewidth - 130;	
    echo '<td valign="top" width="90" bgcolor="'.$tdcolor.'" align="right" style="cursor: default;"><font face="verdana" size="1" color="'.$textcolor.'">'.$file_size.' '.$name[count($name)-1].' &nbsp; <br>'.$imagesize.' &nbsp; </font></td>
    <td width="'.$tdwidth.'" valign="top" bgcolor="'.$tdcolor.'" onclick="openl(\''.$listdir.'/'.$file.'\',\''.$nw.'\',\''.$nh.'\')" onmouseover="this.style.backgroundColor=\''.$tdovercolor.'\'" onmouseout="this.style.backgroundColor=\''.$tdcolor.'\'" style="padding-left: 8;">
    <font face="verdana" size="1" color="'.$textcolor.'"><a style="color: '.$textcolor.'; cursor: default; text-decoration: none;" href="javascript:openl(\''.$listdir.'/'.$file.'\',\''.$nw.'\',\''.$nh.'\')">'.$fileinfo.'</a><br />'.date("r",filemtime($file)).'</font></td></tr>';
  	}
   }
 }
 echo '
 <tr><td colspan="3" bgcolor="'.$tdcolor.'" align="center"><font face="verdana" size="1" color="'.$textcolor.'">Powered by <a href="http://www.tescripts.net/tedls">TEDLS</a>/<a href="http://tech.sfimc.net/">SF-ACTIVE</a></font></td></tr></table></td></tr></table>';

 }

	 function cleanData($data) {
	        // removes characters that break things ;]
	        $data = ereg_replace("-","",$data);
	        $data = ereg_replace(" ","",$data);
	        $data = ereg_replace('\.','',$data);
	        $data = ereg_replace('\'','',$data);
	        return $data;
	}
}
?>
