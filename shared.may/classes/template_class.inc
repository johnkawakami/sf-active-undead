<?php

class FastTemplate {
	//The FastTemplate class is a class that takes a dictionary of fields and 
	//a document with highlighted fields and merges the fields into teh document.


	var $FILELIST	=	array();	
	//	Holds the array of filehandles
	//	FILELIST[HANDLE] == "fileName"

	var $DYNAMIC	=	array();	
	//	Holds the array of dynamic
	//	blocks, and the fileHandles they
	//	live in.

	var $PARSEVARS	=	array();
	//	Holds the array of Variable
	//	handles.
	//	PARSEVARS[HANDLE] == "value"

	var $LOADED		=	array();
	//	We only want to load a template
	//	once - when it's used.
	//	LOADED[FILEHANDLE] == 1 if loaded
	//	undefined if not loaded yet.

	var $HANDLE		=	array();	
	//	Holds the handle names assigned								
	//	by a call to parse()

	var $ROOT		=	"";			
	//	Holds path-to-templates

	var $WIN32		=	false;
	//	Set to true if this is a WIN32 server

	var $ERROR		=	"";
	//	Holds the last error message

	var $LAST		=	"";
	//	Holds the HANDLE to the last
	//	template parsed by parse()

	var $STRICT		=	true;
	//	Strict template checking.
	//	Unresolved vars in templates will
	//	generate a warning when found.
	//	************************************************************

	public $context;

	//function FastTemplate ($pathToTemplates = "")
	function __construct($pathToTemplates = "")
	{
		//The constructor for FastTemplate takes an optional path to
		//the directory that holds the template files.
		global $php_errormsg;
		if(!empty($pathToTemplates))
		{
			$this->set_root($pathToTemplates);
		}
		$this->context = [];
	}
	// end (new) FastTemplate ()

	function set_root ($root)
	{
	// All templates will be loaded from this "root" directory
	// Can be changed in mid-process by re-calling with a new
	// value.	
		$trailer = substr($root,-1);
		if(!$this->WIN32)
		{
			if( (ord($trailer)) != 47 )
			{
				$root = "$root". chr(47);
			}

			if(is_dir($root))
			{
				$this->ROOT = $root;
			}
			else
			{
				$this->ROOT = "";
				$this->error("Specified ROOT dir [$root] is not a directory");
			}
		}
		else
		{
			// WIN32 box - no testing
			if( (ord($trailer)) != 92 )
			{
				$root = "$root" . chr(92);
			}
			$this->ROOT = $root;
		}
	}	// End set_root()

	function utime ()
	{
	// Calculates current microtime
	// I throw this into all my classes for benchmarking purposes
	// It's not used by anything in this class and can be removed
	// if you don't need it.
		$time = explode( " ", microtime());
		$usec = (double)$time[0];
		$sec = (double)$time[1];
		return $sec + $usec;
	}

	function strict ()
	{
	// Strict template checking, if true sends warnings to STDOUT when
	// parsing a template with undefined variable references
	// Used for tracking down bugs-n-such. Use no_strict() to disable.
		$this->STRICT = true;
	}

	function no_strict ()
	{
	// Silently discards (removes) undefined variable references
	// found in templates
		$this->STRICT = false;
	}

	function is_safe ($filename)
	{
	// A quick check of the template file before reading it.
	// This is -not- a reliable check, mostly due to inconsistencies
	// in the way PHP determines if a file is readable.

		if(!file_exists($filename))
		{
			$this->error("[$filename] does not exist",0);
			return false;
		}
		return true;
	}

	function get_template ($template)
	{
	// Grabs a template from the root dir and 
	// reads it into a (potentially REALLY) big string
		if(empty($this->ROOT))
		{
			$this->error("Cannot open template. Root not valid.",1);
			return false;
		}

		$filename	=	"$this->ROOT"."$template";
		$contents = implode("",(@file($filename)));
		if( (!$contents) or (empty($contents)) )
		{
			$this->error("get_template() failure: [$filename] $php_errormsg",1);
		}

		return $contents;
	} // end get_template


	function show_unknowns ($Line)
	{
		// Prints the warnings for unresolved variable references
		// in template files. Used if STRICT is true
		// echo var_dump($Line);
		$unknown = array();
		if (preg_match("/({[A-Z0-9_]+})/",$Line,$unknown))
		{
			$UnkVar = $unknown[1];
			if(!(empty($UnkVar)))
			{
				@error_log("[FastTemplate] Warning: no value found for variable: $UnkVar ",0);
			}
		}
	}	// end show_unknowns()


	function parse_template ($template, $tpl_array)
	{
		// This routine get's called by parse() and does the actual
		// {VAR} to VALUE conversion within the template.
		while ( list ($key,$val) = each ($tpl_array) )
		{
			if (!(empty($key)))
			{
				if(gettype($val) != "string")
				{
					settype($val,"string");
				}
				$template = preg_replace("/{$key}/",$val,$template);
			}
		}

		if(!$this->STRICT)
		{
			// Silently remove anything not already found
			$template = preg_replace("/{[A-Z0-9_]+}/",'',$template);
		}
		else
		{
			// Warn about unresolved template variables
			if (preg_match("/({[A-Z0-9_]+})/",$template))
			{
				$unknown = explode("\n",$template);
				while (list ($Element,$Line) = each($unknown) )
				{
					$UnkVar = $Line;
					if(!(empty($UnkVar)))
					{
						$this->show_unknowns($UnkVar);
					}
				}
			}
		}
		return $template;
	}	// end parse_template();

	function parse_template2 ($template, $tpl_array)
	{
		// This routine get's called by parse() and does the actual
		// {VAR} to VALUE conversion within the template.
		file_put_contents( '/tmp/a', json_encode($tpl_array, JSON_PRETTY_PRINT));
		foreach( $tpl_array as $key=>$val )
		{
			if (!empty($key))
			{
				$template = str_replace($key, "$val", $template);
			}
		}

		if(!$this->STRICT)
		{
			// Silently remove anything not already found
			$template = preg_replace("/[A-Z0-9_]+/",'',$template);
		}
		else
		{
			// Warn about unresolved template variables
			if (preg_match("/([A-Z0-9_]+)/",$template))
			{
				$unknown = explode("\n",$template);
				foreach($unknown as $Element => $Line)
				{
					$UnkVar = $Line;
					if(!(empty($UnkVar)))
					{
						$this->show_unknowns($UnkVar);
					}
				}
			}
		}
		return $template;
	}	// end parse_template();

	function parse2 ( $ReturnVar, $FileTags )
	{
		// The meat of the whole class. The magic happens here.
		$append = false;
		$this->LAST = $ReturnVar;
		$this->HANDLE[$ReturnVar] = 1;

		if (is_array($FileTags))
		{
			unset($this->context[$ReturnVar]);

			foreach ( $FileTags as $key=>$val )
			{
				if ( (!isset($this->context[$val])) || (empty($this->context[$val])) )
				{
					$this->LOADED["$val"] = 1;
					if(isset($this->DYNAMIC["$val"]))
					{
						$this->parse_dynamic($val,$ReturnVar);
					}
					else
					{
						$fileName = $this->FILELIST["$val"];
						$this->context[$val] = $this->get_template($fileName);
					}
				}

				//	Array context implies overwrite
				$this->context[$ReturnVar] = $this->parse_template2($this->context[$val], $this->PARSEVARS);

				//	For recursive calls.
				$this->assign( array( $ReturnVar => $this->context[$ReturnVar] ) );

			}
		}	// end if FileTags is array()
		else
		{
			// FileTags is not an array
			$val = $FileTags;

			if( (substr($val,0,1)) == '.' )
			{
				// Append this template to a previous ReturnVar
				$append = true;
				$val = substr($val,1);
			}

			if ( (!isset($this->context[$val])) || (empty($this->context[$val])) )
			{
				$this->LOADED["$val"] = 1;
				if(isset($this->DYNAMIC["$val"]))
				{
					$this->parse_dynamic($val,$ReturnVar);
				}
				else
				{
					$fileName = $this->FILELIST["$val"];
					$this->context[$val] = $this->get_template($fileName);
				}
			}

			if($append)
			{
				$this->context[$ReturnVar] .= $this->parse_template2($this->context[$val], $this->PARSEVARS);
			}
			else
			{
				$this->context[$ReturnVar] = $this->parse_template2($this->context[$val], $this->PARSEVARS);
			}

			//	For recursive calls.
			$this->assign(array( $ReturnVar => $this->context[$ReturnVar]) );
			
		}
		return;
	}	//	End parse2()

	function parse ( $ReturnVar, $FileTags )
	{
		//echo var_dump($ReturnVars, $FileTags);
		// The meat of the whole class. The magic happens here.
		$append = false;
		$this->LAST = $ReturnVar;
		$this->HANDLE[$ReturnVar] = 1;

		if (gettype($FileTags) == "array")
		{
			unset($this->$ReturnVar);	// Clear any previous data

			while ( list ( $key , $val ) = each ( $FileTags ) )
			{
				if ( (!isset($this->$val)) || (empty($this->$val)) )
				{
					$this->LOADED["$val"] = 1;
					if(isset($this->DYNAMIC["$val"]))
					{
						$this->parse_dynamic($val,$ReturnVar);
					}
					else
					{
						$fileName = $this->FILELIST["$val"];
						$this->$val = $this->get_template($fileName);
					}
				}

				//	Array context implies overwrite
				$this->$ReturnVar = $this->parse_template($this->$val,$this->PARSEVARS);

				//	For recursive calls.
				$this->assign( array( $ReturnVar => $this->$ReturnVar ) );

			}
		}	// end if FileTags is array()
		else
		{
			// FileTags is not an array
			$val = $FileTags;

			if( (substr($val,0,1)) == '.' )
			{
				// Append this template to a previous ReturnVar
				$append = true;
				$val = substr($val,1);
			}

			if ( (!isset($this->$val)) || (empty($this->$val)) )
			{
				if ($val == '$0') {
					throw new \Exception('val is 0');
				}
				$this->LOADED["$val"] = 1;
				if(isset($this->DYNAMIC["$val"]))
				{
					$this->parse_dynamic($val,$ReturnVar);
				}
				else
				{
					$fileName = $this->FILELIST["$val"];
					$this->$val = $this->get_template($fileName);
				}
			}

			if($append)
			{
				$this->$ReturnVar .= $this->parse_template($this->$val,$this->PARSEVARS);
			}
			else
			{
				$this->$ReturnVar = $this->parse_template($this->$val,$this->PARSEVARS);
			}

			//	For recursive calls.
			$this->assign(array( $ReturnVar => $this->$ReturnVar) );
			
			// echo var_dump($this);
		}
		return;
	}	//	End parse()

	function FastPrint ( $template = "" )
	{
		//prints a template given a template name
		if(empty($template))
		{
			$template = $this->LAST;
		}

		if( (!(isset($this->$template))) || (empty($this->$template)) )
		{
			$this->error("Nothing parsed, nothing printed",0);
			return;
		}
		else
		{
			echo $this->$template;
		}

		return;

	} 

	function FastPrint2 ( $template = "" )
	{
		//prints a template given a template name
		if(empty($template))
		{
			$template = $this->LAST;
		}

		if( (!(isset($this->context[$template]))) || (empty($this->context[$template])) )
		{
			$this->error("Nothing parsed, nothing printed",0);
			return;
		}
		else
		{
			echo $this->context[$template];
		}

		return;

	}


	function fetch ( $template = "" )
	{
		//returns a template given a template name
		if(empty($template))
		{
			$template = $this->LAST;
		}

		if( (!(isset($this->$template))) || (empty($this->$template)) )
		{
			$this->error("Nothing parsed, nothing printed",0);
			return "";
		}

		return($this->$template);
	}

	function fetch2( $template = "" )
	{
		//returns a template given a template name
		if(empty($template))
		{
			$template = $this->LAST;
		}

		if( (!(isset($this->context[$template]))) || (empty($this->context[$template])) )
		{
			$this->error("Nothing parsed, nothing printed",0);
			return "";
		}

		return($this->context[$template]);
	}


	function define_dynamic ($Macro, $ParentName)
	{
		//	A dynamic block lives inside another template file.
		//	It will be stripped from the template when parsed
		//	and replaced with the {$Tag}.
		$this->DYNAMIC["$Macro"] = $ParentName;
		return true;
	}

//	************************************************************

	function parse_dynamic ($Macro,$MacroName)
	{
     	// Parses a file that is already in memory.
		$ParentTag = $this->DYNAMIC["$Macro"];
		if( (!$this->$ParentTag) or (empty($this->$ParentTag)) )
		{
			$fileName = $this->FILELIST[$ParentTag];
			$this->$ParentTag = $this->get_template($fileName);
			$this->LOADED[$ParentTag] = 1;
		}

		if($this->$ParentTag)
		{
			$template = $this->$ParentTag;
			$DataArray = explode("\n",$template);
			$newMacro = "";
			$newParent = "";
			$outside = true;
			$start = false;
			$end = false;

			while ( list ($lineNum,$lineData) = each ($DataArray) )
			{
				$lineTest = trim($lineData);
				if("<!-- BEGIN DYNAMIC BLOCK: $Macro -->" == "$lineTest" )
				{
					$start = true;
					$end = false;
					$outside = false;
				}

     			if("<!-- END DYNAMIC BLOCK: $Macro -->" == "$lineTest" )
				{
					$start = false;
					$end = true;
					$outside = true;
				}

				if( (!$outside) and (!$start) and (!$end) )
				{
					$newMacro .= "$lineData\n"; // Restore linebreaks
				}

				if( ($outside) and (!$start) and (!$end) )
				{
					$newParent .= "$lineData\n"; // Restore linebreaks
				}

				if($end)
				{
					$newParent .= "{$MacroName}\n";
				}

				// Next line please
				if($end) { $end = false; }
				if($start) { $start = false; }
			}	// end While

			$this->$Macro = $newMacro;
			$this->$ParentTag = $newParent;
			return true;
		}	// $ParentTag NOT loaded - MAJOR oopsie
		else
		{
			@error_log("ParentTag: [$ParentTag] not loaded!",0);
			$this->error("ParentTag: [$ParentTag] not loaded!",0);
		}
		return false;
	}

	function clear_dynamic ($Macro="")
	{
     	// Strips a DYNAMIC BLOCK from a template.
		if(empty($Macro)) { return false; }

          // The file must already be in memory.
		$ParentTag = $this->DYNAMIC["$Macro"];

		if( (!$this->$ParentTag) or (empty($this->$ParentTag)) )
		{
			$fileName = $this->FILELIST[$ParentTag];
			$this->$ParentTag = $this->get_template($fileName);
			$this->LOADED[$ParentTag] = 1;
		}

		if($this->$ParentTag)
		{
			$template = $this->$ParentTag;
			$DataArray = explode("\n",$template);
			$newParent = "";
			$outside = true;
			$start = false;
			$end = false;
			while ( list ($lineNum,$lineData) = each ($DataArray) )
			{
				$lineTest = trim($lineData);
				if("<!-- BEGIN DYNAMIC BLOCK: $Macro -->" == "$lineTest" )
				{
					$start = true;
					$end = false;
					$outside = false;
				}

				if("<!-- END DYNAMIC BLOCK: $Macro -->" == "$lineTest" )
				{
					$start = false;
					$end = true;
					$outside = true;
				}

				if( ($outside) and (!$start) and (!$end) )
				{
					$newParent .= "$lineData\n"; // Restore linebreaks
				}

				// Next line please
     			if($end) { $end = false; }
				if($start) { $start = false; }
			}	// end While

			$this->$ParentTag = $newParent;
			return true;
		}	// $ParentTag NOT loaded - MAJOR oopsie
		else
		{
			@error_log("ParentTag: [$ParentTag] not loaded!",0);
			$this->error("ParentTag: [$ParentTag] not loaded!",0);
		}
		return false;
	}

	function define ($fileList)
	{
		//defines a file list
		while ( list ($FileTag,$FileName) = each ($fileList) )
		{
			$this->FILELIST["$FileTag"] = $FileName;
		}
		return true;
	}


	function clear_parse ( $ReturnVar = "")
	{
		//clears the buffer
		$this->clear($ReturnVar);
	}

	function clear ( $ReturnVar = "" )
	{
     	// Clears out hash created by call to parse()
		if(!empty($ReturnVar))
		{
			if( (gettype($ReturnVar)) != "array")
			{
				unset($this->$ReturnVar);
				return;
			}
			else
			{
				while ( list ($key,$val) = each ($ReturnVar) )
				{
					unset($this->$val);
				}
				return;
			}
		}

		// Empty - clear all of them
		while ( list ( $key,$val) = each ($this->HANDLE) )
		{
			$KEY = $key;
			unset($this->$KEY);
		}
		return;
	}	//	end clear()

//	************************************************************

     function clear_all ()
	{
     	//reinitializes the class
		$this->clear();
		$this->clear_assign();
		$this->clear_define();
		$this->clear_tpl();
		return;
	}	//	end clear_all

//	************************************************************

	function clear_tpl ($fileHandle = "")
	{
		//clears out teh template stored in teh class

		if(empty($this->LOADED))
		{
			// Nothing loaded, nothing to clear
			return true;
		}

		if(empty($fileHandle))
		{
			// Clear ALL fileHandles
			while ( list ($key, $val) = each ($this->LOADED) )
			{
				unset($this->$key);
			}
			unset($this->LOADED);
			return true;
		}
		else
		{
			if( (gettype($fileHandle)) != "array")
			{
				if( (isset($this->$fileHandle)) || (!empty($this->$fileHandle)) )
				{
					unset($this->LOADED[$fileHandle]);
					unset($this->$fileHandle);
					return true;
				}
			}
			else
			{
				while ( list ($Key, $Val) = each ($fileHandle) )
				{
					unset($this->LOADED[$Key]);
					unset($this->$Key);
				}
				return true;
			}
		}
		return false;
	}	// end clear_tpl

//	************************************************************

	function clear_define ( $FileTag = "" )
	{
		//unsets the file list in memory
		
		if(empty($FileTag))
		{
			unset($this->FILELIST);
			return;
		}

     	if( (gettype($Files)) != "array")
		{
			unset($this->FILELIST[$FileTag]);
			return;
		}
		else
		{
			while ( list ( $Tag, $Val) = each ($FileTag) )
			{
				unset($this->FILELIST[$Tag]);
			}
			return;
		}
	}

	function clear_assign ()
	{
		//	Clears all variables set by assign()
		if(!(empty($this->PARSEVARS)))
		{
			while(list($Ref,$Val) = each ($this->PARSEVARS) )
			{
				unset($this->PARSEVARS["$Ref"]);
			}
		}
	}

	function clear_href ($href)
	{
		//unsets PARSEVARS
		if(!empty($href))
		{
			if( (gettype($href)) != "array")
			{
				unset($this->PARSEVARS[$href]);
				return;
			}
			else
			{
				while (list ($Ref,$val) = each ($href) )
				{
					unset($this->PARSEVARS[$Ref]);
				}
				return;
			}
		}
		else
		{
			// Empty - clear them all
			$this->clear_assign();
		}
		return;
	}

	function assign ($tpl_array, $trailer="")
	{
		//sets PARSEVARS from the tpl_array
		if(gettype($tpl_array) == "array")
		{
			while ( list ($key,$val) = each ($tpl_array) )
			{
				if (!(empty($key)))
				{
					//	Empty values are allowed
					//	Empty Keys are NOT
					$this->PARSEVARS["$key"] = $val;
				}
			}
		}
		else
		{
			// Empty values are allowed in non-array context now.
			if (!empty($tpl_array))
			{
				$this->PARSEVARS["$tpl_array"] = $trailer;
			}
		}
	}

	function get_assigned($tpl_name = "")
	{
		//	Return the value of an assigned variable.
		if(empty($tpl_name)) { return false; }
		if(isset($this->PARSEVARS["$tpl_name"]))
		{
			return ($this->PARSEVARS["$tpl_name"]);
		}
		else
		{
			return false;
        }
	}

	function error ($errorMsg, $die = 0)
	{
		//sets an Error with an optional message
		$this->ERROR = $errorMsg;
		echo "ERROR: $this->ERROR <BR> \n";
		if ($die == 1)
		{
			exit;
		}
		return;
	} // end error()

	function render_template($variable_array,$template_name)
	{
		//global $template_obj;
		$this->clear_all();
		$this->define(array('thisfile'=>$template_name));
		$this->assign($variable_array);
		$this->parse('HTML',"thisfile");
		$result_html=$this->fetch("HTML");
		return $result_html;
	}
} // End class.FastTemplate.php3
     
