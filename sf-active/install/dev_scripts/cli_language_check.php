<?

// translation checking script

// this script will compare the dict files from a certain language to the english one.
// it will count if:
//      - all files are there & which ones are missing.
//      - see if there are strings missing in an array.
//
//
// usage:
//
//      path/to/php cli_language_check.php [languagecode] [save]
//	[ save will save the output under a file [languagecode] in the dir you're running the script]

set_time_limit(0);
ini_set('display_errors', true);
ini_set('include_path', '.:/www/sf-active');

// define some things
define(SF_SHARED_PATH, '/www//sf-active//shared');
define(SF_DICT_PATH, SF_SHARED_PATH.'/dictionary');

// start of the script
if(!$_SERVER['argv']['1'])
{
        echo("SF-Active Language Check Script\n\n");
        echo("Usage:\n");
        echo("path/to/php cli_language_check.php [languagecode] [save]\n");
	echo("[ save will save the output under a file [languagecode] in the dir you're running the script]\n");
        exit;
}

$en_path = SF_DICT_PATH.'/en/';
$lang_path = SF_DICT_PATH.'/'.$_SERVER['argv']['1'] ;

// check all the files
$file_mem = array();
$file_list_en = opendir($en_path);
while($list = readdir($file_list_en))
{
	if(!file_exists($lang_path."/$list"))
	{
		echo("You're missing file $list\n");
		$logfile .= ("You're missing file $list\n");
	}
}

// now we have checked if all files exist, we go for the strings 
$file_list_en = opendir($en_path);

while($list = readdir($file_list_en))
{
	// we only do existing files in the to-check language,
	// since we listed to others allready
	
	if((file_exists($lang_path.'/'.$list)) && (!is_dir($lang_path.'/'.$list)) && ($list !== "common.dict"))
	{
		include($en_path.'/'.$list);
		$check_array = $local ;		
		include($lang_path.'/'.$list);
		foreach($check_array as $key => $value)
		{
			if(!array_key_exists($key, $local))
			{
				echo("in $list you miss '$key' \n");
				$logfile .= "in $list you miss '$key' \n";
			}
			elseif($local["$key"] == $value)
			{
				 echo("in $list, '$key' is not translated \n");
                                 $logfile .= "in $list, '$key' is not translated \n";
			}
		}
	}
	elseif($list == "common.dict")
	{
		unset($check_array);
		unset($local);
		include($en_path.'/common.dict');
		foreach($GLOBALS['dict'] as $key => $value)
		{
			$check_array[$key] = $value;
			$GLOBALS['dict'][$key] = '';
		}
		include($lang_path.'/common.dict');
		foreach($GLOBALS['dict'] as $key => $value)
		{
			$local[$key] = $value ;
		}
		foreach($check_array as $key => $value)
		{
			if((!array_key_exists($key, $local)) || ($local["$key"] == ''))
			{
				echo("in common.dict you miss '\$GLOBALS['dict'][$key']' \n");
				$logfile .= "in common.dict you miss '\$GLOBALS['dict'][$key]' \n";
			}	
			elseif($local["$key"] == $value)
			{
				echo("in common.dict, '\$GLOBALS['dict']['$key']' is not translated\n");
				$logfile .= "in common.dict, '\$GLOBALS['dict']['$key']' is not translated\n";
			}
		}
	}
}

if($_SERVER['argv']['2'] == "save")
{
	include_once(SF_SHARED_PATH . '/classes/cache_class.inc');
	$cache = new Cache;
	$cache->cache_file($logfile, './'.$_SERVER['argv']['1']);
}



?>
