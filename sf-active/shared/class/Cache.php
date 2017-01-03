<?php

class Cache
{
    // A parent class which handles caching and filesystem functions
    // Also handles form input data

    var $cachestatus;        // An HTML-ready display of cache notices
    var $errorstatus;        // An HTML-ready display of error notices
    var $upload_target_url;  // Temporary array for upload targets

    function update_status($update_string)
    {
        // Allows easy access to adding an update string
        $this->cachestatus = str_replace("<UL>", "", $this->cachestatus);
        $this->cachestatus = str_replace("</UL>", "", $this->cachestatus);
        $this->cachestatus .= "<LI>" . $update_string . "</LI>";
        $this->cachestatus = "<UL>" . $this->cachestatus . "</UL>";
        return 1;
    }

    function clear_update_status()
    {
        // Clears any existing update status
        $this->cachestatus = "";
        return 1;
    }

    function get_update_status()
    {
        return $this->cachestatus;
    }

    function update_error_status($update_string)
    {
        // Allows easy access to adding an update string
        $this->errorstatus = str_replace("<UL>", "", $this->errorstatus);
        $this->errorstatus = str_replace("</UL>", "", $this->errorstatus);
        $this->errorstatus .= "<LI>" . $update_string . "</LI>";
        $this->errorstatus = "<UL>" . $this->errorstatus . "</UL>";
        return 1;
    }

    function clear_error_status()
    {
        // Clears any existing error status
        $this->errorstatus = "";
        return 1;
    }

    function get_error_status()
    {
        return $this->errorstatus;
    }

    function sf_error_log($error)
    {
        $timestamp = date("h:i D n M Y");
        $file = $_SERVER['SCRIPT_NAME'];
        $log_message = "[" . $timestamp . "]" . " $file, " . $error . "\n";
        error_log($log_message, SF_ERR_TYPE, SF_ERR_LOG);
        return 1;
    }         

    function display_error_status()
    {
        $tr = new Translate;
        $html = "<p class=\"error\"><strong>";
        $html .= $tr->trans('error_detected') . "</strong><br />";
        $html .= $tr->trans('fill_out_form_right') . ":<br />";
        $html .= $this->errorstatus;
        return $html;
    }

    function is_error()
    {
        // Returns true if there is an existing error
        if (strlen($this->errorstatus) > 0)
        {
            return 1;
        } else
        {
            return 0;
        }
    }

    function cache_file($html,$filename)
    {
        $fp = fopen($filename,"w");
        flock($fp,2);
        fwrite($fp,$html);
        flock($fp,3);
        fclose($fp);
    }

    function render_yearmonth_link($new_year,$new_month)
    {
        //returns a file path given a year and month
        $Month = $new_month;
        $Year = $new_year;
        $pathtofile = "$Year/$Month/";
        return $pathtofile;
    }

    function render_protest_date($created)
    {
        //This creates the protest-style dates, like J18 + the time
        $returnval = substr_replace($created, '', 1,3);
        return $returnval;
    }

    function render_entities($html)
    {
        $html=str_replace(array("'","<",">"),array("&#039;","&lt;","&gt;"),$html);
        return $html;
    }

    function cleanup_post()
    {
        // Removes dangerous HTML from data submitted via POST
        while (list($key,$value) = each($_POST))
        {
            $_POST["$key"] = preg_replace("/\"/", "&quot;", $_POST["$key"]);
//          if (preg_match_all("/<(\?|\%|script)(.|\n)*/", $_POST[$key], $matches))
//          {
                $_POST["$key"] = preg_replace("/<(\?|\%|script)(.|\n)*/","",$_POST["$key"]);
//          }
        }
    }

     function validate_post()
     {
          // Error checking for data submitted via POST

          $tr = new Translate;

          $this->clear_error_status();
          if (strlen(chop($_POST["heading"])) == 0)
               $this->update_error_status($tr->trans('pub_error_title'));

          if (strlen(chop($_POST["author"])) == 0)
               $this->update_error_status($tr->trans('pub_error_author'));

          if (chop($_POST["publish_type"]) == "publish")
          {
               if (strlen(chop($_POST["summary"])) < 1)
                    $this->update_error_status($tr->trans('pub_error_summary'));

               if (strlen(chop($_POST["summary"])) > $GLOBALS['summary_maxsize'])
                    $this->update_error_status($tr->trans('pub_error_long_summary'));
          }

          if ($_FILES["linked_file_1"]["name"] == "" && strlen(chop($_POST["article"])) < 10)
               $this->update_error_status($tr->trans('pub_error_upload'));

          if ($_POST["file_count"] > 20)
               $this->update_error_status($tr->trans('pub_error_long_upload'));

	  if(strlen($_POST['language_id']) == 0 )
		$this->update_error_status($tr->trans('pub_error_language'));

          $this->validate_uploads();

          if ($this->is_error())
          {
               return 0;
          } else {
               return 1;
          }
     }

     function validate_event_post()
     {
	$tr = new Translate ;
	$this->clear_error_status();
	$this->validate_uploads();
	if(!$_POST['language_id'])
                $this->update_error_status($tr->trans('cal_error_language'));
        if ($this->is_error())
        {
             return 0;
        } else {
             return 1;
        }	
     }

     function validate_uploads()
     {

          $tr = new Translate;
          // Error checking for files submitted via POST
          if (($_POST["file_count"] <= 20))
          {
               // Above check makes sure we are not spinning forever
               for ($counter = 1; $_POST["file_count"] >= $counter; $counter++)
               {
                    $this->sf_error_log("we have a problem houston");                    
                    // Check if we even have uploads
                    if (($_FILES["linked_file_".$counter]["name"] != "" ) && 
                        ($_FILES["linked_file_".$counter]["name"] != "none"))
                    {
                         $file_extension = $_FILES["linked_file_".$counter]["name"];
                         $clean_basename = strtolower(ereg_replace("[^a-zA-Z0-9._-]", "_", $_FILES["linked_file_".$counter]["name"]));
                         $upload_target = $GLOBALS["upload_folder"] . $clean_basename;
                         eregi("([a-z0-9/\._-]+)\.([a-z0-9]+)$", $upload_target, $regs);
                         $upload_ext = $regs[2];

                         // Here is where we auto-detect uploads, thanks to occam!
                         switch ($upload_ext)
                         {
                              case "txt":
                                   $next_mime_type="text/plain";
                                   break;
                              case "html":
                              case "htm":
                                   $next_mime_type="text/html";
                                   break;
                              case "jpg":
                              case "jpeg":
                              case "jpe":
                                   $next_mime_type="image/jpeg";
                                   break;
                              case "gif":
                                   $next_mime_type="image/gif";
                                   break;
                              case "png":
                                   $next_mime_type="image/png";
                                   break;
                              case "mp3":
                              case "mp2":
                              case "mpga":
                                   $next_mime_type="audio/mpeg";
                                   break;
                              case "m3u":
                                   $next_mime_type="audio/x-mpegurl";
                                   break;
                              case "pls":
                                   $next_mime_type="audio/x-scpls";
                                   break;
                              case "ram":
                                   $next_mime_type="video/x-pn-realvideo-meta";
                                   break;
                              case "rm":
                                   $next_mime_type="audio/x-pn-realaudio";
                                   $realfile=fopen($_FILES["linked_file_".$counter]["tmp_name"], "r");
                                   $contents = fread ($realfile, 2000);
                                   if (strpos($contents, "realvideo")>0) $next_mime_type = "video/x-pn-realvideo";
                                   fclose($realfile);
                                   break;
                              case "ra":
                                   $next_mime_type="audio/x-pn-realaudio";
                                   break;
                              case "wav":
                                   $next_mime_type="audio/x-wav";
                                   break;
                              case "3gp":
                              case "mp4":
                              case "mpg":
                              case "mpeg":
                              case "mpe":
                                   $next_mime_type="video/mpeg";
                                   break;
                              case "avi":
                                   $next_mime_type="video/x-msvideo";
                                   break;
                              case "mov":
                              case "qt":
                              case "qtl":
                                   $next_mime_type="video/quicktime";
                                   break;
                              case "pdf":
                                   $next_mime_type="application/pdf";
                                   break;
                              case "wma":
                                   $next_mime_type="audio/x-ms-wma";
                                   break;
                              case "wmv":
                                   $next_mime_type="video/x-ms-wmv";
                                   break;
                              case "ogg":
                                   $next_mime_type="audio/x-ogg";
                                   break;
							  case "ogv":
								   $next_mime_type="video/x-ogg";
								   break;
                              case "torrent":
                                   $next_mime_type="application/x-bittorrent";
                                   break;
			      case "swf":
				   $next_mime_type="application/x-shockwave-flash";
				   break;
			      case "rtf":
				   $next_mime_type="application/rtf";
				   break;
                              default:
                                   $next_mime_type="";
                         }

                         $_POST["mime_type_file_".$counter] = $next_mime_type;

                         // Unknown MIME-Type
                         if ($_POST["mime_type_file_".$counter] == "")
                         {
                              $this->update_error_status($tr->trans('pub_error_file'));
                         } else
                         {
                              // Check Upload Data
                              // Image Check
                        /* why do this here ? --st3
			      if ((substr($_POST["mime_type_file_".$counter],0,5)=="image" ) && 
                                 (!@GetImageSize( $_FILES["linked_file_".$counter]["tmp_name"])))
                              {
                                        $this->update_error_status($tr->trans('pub_error_where_file'));
                              }
			*/
                              if (substr($_POST["mime_type_file_".$counter],0,5)=="image")
                              {
								  include_once(SF_SHARED_PATH."/classes/image_class.inc");
								  include_once(SF_SHARED_PATH."/classes/image2_class.inc");
                                  $image=new Image2();
                                  if (!$image->validate_image($_FILES["linked_file_".$counter]["tmp_name"],$upload_ext) )
                                  $this->update_error_status($tr->trans('pub_error_where_file'));
                              }


                              // Upload Title Check
                              if ($counter > 1 && $_POST["linked_file_title_".$counter] == "" )
                              {
                                   $tmperr = $tr->trans('pub_error_select_file') . " #".$counter.".";
                                   $this->update_error_status("$tmperr");
                              }
                         }
                    } else
                    {
                         // No File Selected...
                         $this->update_error_status($tr->trans('pub_error_select_file')." #".$counter.".");
                    }
               }
          }
     } // end function

     function process_uploads()
     {
          // Process any uploads from a POST

          $tr = new Translate;

          $tmpupd = $tr->trans('there_were') . " " . $_POST['file_count'] . " " . $tr->trans('files_detected');
          $this->update_status($tmpupd);

          for ($counter = 1; $_POST["file_count"] >= $counter; $counter++)
          {

		// holy shit, must rewrite this logic -johnk
               if (is_uploaded_file($tmpfilename = $_FILES["linked_file_".$counter]["tmp_name"]))
               {
					# if ($this->check_evil_file($tmpfilename)) break;

					if(!$this->article['created_year'] || !$this->article['created_month'])
					{
					$this->article['created_year'] = date(Y);
					$this->article['created_month'] = date(m);
					}
                    $this->create_upload_dirs();


                    $clean_basename = ereg_replace("[^a-zA-Z0-9._-]","_", $_FILES["linked_file_".$counter]["name"]);
                    $clean_basename = strtolower($clean_basename);
                    $upload_target = SF_UPLOAD_PATH ."/".$this->article['created_year']."/".$this->article['created_month']."/" . $clean_basename;

                    eregi("([a-z0-9/\._-]+)\.([a-z0-9]+)$", $upload_target, $regs);
                    $upload_ext = $regs[2];

                    if (strlen($upload_ext) == 0)
                    {
                         $upload_ext = $mime_extensions[chop($_POST["mime_type_file_".$counter])];
                         $clean_basename .= "." . $upload_ext;
                         $upload_target = SF_UPLOAD_PATH ."/".$this->article['created_year']."/".$this->article['created_month']."/" . $clean_basename;
                     }
                      // need to check if we are about to wipe another file!
                     // but if we have a thumbnailed image, we workaround
                     if (file_exists($_FILES['linked_file_'.$counter]['tmp_name'].'mid')  || file_exists($_FILES['linked_file_'.$counter]['tmp_name'].'thumb')) {
						$upload_target_mid = $upload_target."mid.".$upload_ext;
						$upload_target_thumb = $upload_target."thumb.".$upload_ext;
                        while (file_exists($upload_target) || file_exists($upload_target.'mid.'.$upload_ext) || file_exists($upload_target.'thumb.'.$upload_ext))
                        {
                             $tmpname=tempnam(SF_UPLOAD_PATH. "/".$this->article['created_year']."/".$this->article['created_month']."/",$clean_basename);
                             unlink($tmpname);
                             $tmpname=strtolower($tmpname);
                             $upload_target = $tmpname.".".$upload_ext;
                             if (file_exists($_FILES['linked_file_'.$counter]['tmp_name'].'mid')) $upload_target_mid = $tmpname.".".$upload_ext."mid.".$upload_ext;
                             if (file_exists($_FILES['linked_file_'.$counter]['tmp_name'].'thumb')) $upload_target_thumb = $tmpname.".".$upload_ext."thumb.".$upload_ext;
                        }

                        if (!move_uploaded_file($_FILES["linked_file_".$counter]["tmp_name"],$upload_target)) // || (!rename($_FILES["linked_file_".$counter]["tmp_name"].'mid' , $upload_target_mid)))
                        {
                                // when we have better error handling, change this next line
                                echo "<strong>FATAL ERROR</strong>: Move failed! No disk space? :(\n";
                                exit;
                         } else {
                                 chmod ($upload_target, 0644);
				if(file_exists($_FILES["linked_file_".$counter]["tmp_name"].'mid')){ 
				    copy($_FILES["linked_file_".$counter]["tmp_name"].'mid' , $upload_target_mid) && unlink($_FILES["linked_file_".$counter]["tmp_name"].'mid');
				    chmod ($upload_target_mid, 0644); }
				if (file_exists($_FILES['linked_file_'.$counter]['tmp_name'].'thumb')) {
                                    copy($_FILES['linked_file_'.$counter]['tmp_name'].'thumb',$upload_target_thumb) && unlink($_FILES['linked_file_'.$counter]['tmp_name'].'thumb');
                                    chmod ($upload_target_thumb, 0644);
                               }
                         }


                    } else {
                    // need to check if we are about to wipe another file!
                    while (file_exists($upload_target))
                    {
			 $tmpname=tempnam(SF_UPLOAD_PATH."/".$this->article['created_year']."/".$this->article['created_month'],$clean_basename);
                         unlink($tmpname);
			// i hope the path is all lower case!
                         $tmpname=strtolower($tmpname);
                         $upload_target = $tmpname.".".$upload_ext;
                    }

                    // copy it to our upload directory
                    if (!move_uploaded_file($_FILES["linked_file_".$counter]["tmp_name"],$upload_target))
                    {
                         // when we have better error handling, change this next line
                         echo "<strong>FATAL ERROR</strong>: Move failed! No disk space? :(\n";
                         exit;
                    } else
                    {
                         chmod ($upload_target, 0644);
                    }
		 }
                    // if necessary, cache a ram file and then update our progress
                    if (chop($_POST["mime_type_file_".$counter]) == "audio/x-pn-realaudio" || 
                        chop($_POST["mime_type_file_".$counter]) == "video/x-pn-realvideo" )
                    {
                              $this->cache_ram_file($upload_target, $this->article['created_year'], $this->article['created_month']);
                    }

                    $this->upload_target_url[$counter]=$upload_target;
                    $tmpupdate = "Received file: " . $clean_basename;
                    $tmpupdate .= ", type: " . $_POST["mime_type_file_".$counter];
                    $tmpupdate .= ", size: " . filesize($upload_target);
                    $this->update_status("$tmpupdate");

                    // mtoups -- media mirroring code
                    if (file_exists($upload_target) && 
		        file_exists($GLOBALS['mirroring_script_path']) )
		    {
                      $output = system("sh " . $GLOBALS['mirroring_script_path'] . " " . $upload_target, $retval);
		      //echo "rsync says: " . $output . ", return value ". $retval;
                    }

               }
          }
     }

     function cache_ram_file($upload_target, $year, $month)
     {
          // Cache a RAM file to disk during uploads
          $ram_filename=eregi_replace("\.r[avm]$",'.ram',basename($upload_target));
          $rm_file=basename($upload_target);
          // construct a rtsp: or pnm: url with: base_url + clean_basename
          $rtsp_url = $GLOBALS['rtsp_base_url'] ."/".$year."/".$month. "$rm_file";
          $pnm_url  = $GLOBALS['pnm_base_url'] ."/".$year."/".$month. "$rm_file";
          $target_filename = SF_UPLOAD_PATH ."/".$year."/".$month. "/$target_filename";
          $ram_dest = SF_UPLOAD_PATH ."/".$year."/".$month."/".$ram_filename;

          // now assemble the file and cache it
          $ramfile = $rtsp_url . "\n--stop--\n" . $pnm_url;
          $this->cache_file($ramfile, $ram_dest);
          return 1;
     }

     function do_form($form_name, $options, $match_array=array(), $match)
     {
	if($GLOBALS['multicat_form_style'] == "select")
	{
	    $form = $this->make_select_form($form_name, $options, $match);
	}
	elseif($GLOBALS['multicat_form_style'] == "multiple")
	{
	    $form = $this->make_multiple_form($form_name, $options, $match_array);
	}
	elseif($GLOBALS['mulitcat_form_style'] = "checkbox")
	{
	    unset($options['0']);
	    $form = $this->make_checkbox_form($form_name, $options, $match_array);
	}
	return $form;
    }

     function make_select_form ($select_name, $options, $match)
     {
          $return_html = "<select name=\"" . $select_name . "\">\n";
          while (list($key, $value) = each ($options))
          {
               $return_html .= "<option value=\"$key\"";
               if ($match == $key)
               {
                $return_html .= " selected=\"selected\"";
               }
               $return_html .= ">$value</option>\n";
          }
          $return_html .= "</select>\n";
          return $return_html;
     }

     function make_multiple_form($select_name, $options, $match)
     {
	if(!$GLOBALS['multicat_multiple_size']) { $GLOBALS['multicat_multiple_size'] = 7 ; }
	$return_html = "<select name=\"".$select_name."\" multiple size=\"".$GLOBALS['multicat_multiple_size']."\">\n";
	while(list($key, $value) = each($options))
	{
	    $return_html .= "<option value=\"".$key."\"";
	    if(is_array($match))
	    {
		if(in_array($key, $match) && $key !== 0)
		{
		   $return_html .= " selected=\"selected\"";
		}
	    }
	    $return_html .= ">".$value."</option>\n";
	}
	$return_html .= "</select>\n";
	return $return_html;
    }

     function make_checkbox_form($checkbox_name, $options, $checked)
     {
          while (list($key, $value) = each ($options))
          {
               $return_html .= "<input type=\"checkbox\" name=\"" . $checkbox_name . "\"";
               $return_html .= " value=\"" . $key . "\"";

               if (is_array($checked))
               {
                    if (in_array($key,$checked))
                    {
                         $return_html .= " checked=\"checked\"";
                    }
               }

               $return_html .= " /> " . $value . "&nbsp;&nbsp;&nbsp;\n";
          }
          return $return_html;
    }

    function create_upload_dirs()
    {
        if (!is_dir(SF_UPLOAD_PATH . "/" . $this->article['created_year']))
        {
            mkdir(SF_UPLOAD_PATH . "/" . $this->article['created_year'],0777);
            chmod(SF_UPLOAD_PATH . "/" . $this->article['created_year'],0777);
        }
        if (!is_dir(SF_UPLOAD_PATH . "/" . $this->article['created_year']."/".$this->article['created_month']))
        {
            mkdir(SF_UPLOAD_PATH . "/" . $this->article['created_year']."/".$this->article['created_month'],0777);
            chmod(SF_UPLOAD_PATH . "/" . $this->article['created_year']."/".$this->article['created_month'],0777);
        }
    }

    function check_evil_file($fname)
    {
	$fh = fopen($fname,"r");
	fclose( $fh );
    }

} //end class

function MakeCacheDir($ts_date)
{
     // okay, adding in identification for the 2 types of dates we might have

     if (ereg ("([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})", $ts_date))
     {
          $Month = substr($ts_date,5,2);
          $Year = substr($ts_date,0,4);
     } else
     {
          $Month = substr($ts_date,4,2);
          $Year = substr($ts_date,0,4);
          $pathtofile = "$Year/$Month/";
          return $pathtofile;
     }
     $pathtofile="$Year/$Month/";
     return $pathtofile;
}

function MakeCacheDirYearMonth($new_year,$new_month)
{
     //returns a file path given a year and month
     $Month = $new_month;
     $Year = $new_year;
     $pathtofile = "$Year/$Month/";
     return $pathtofile;
}

?>
