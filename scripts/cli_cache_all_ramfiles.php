<?php

// this script is used to recache all your ramfiles from the command line.
// this script runs as the user who's executing it, not as the user apache uses.
// this means it will only work if your user has write access to files in SF_UPLOAD_PATH

// to regenerate all ramfiles:
// /usr/bin/php cli_cache_ramfiles.php all
// to regenerate some ramfiles:
// /usr/bin/php cli_cache_ramfiles.php [starting_id] [amount of files]


// you need to edit the following settings in order to make it work.
ini_set('include_path', '.:/www/sf-active:');
$_SERVER['SITE_NAME'] = 'sitename' ;
include('shared/global.cfg');

// start of the script
set_time_limit(0);
ini_set('display_errors', true);

// Set up a timer so we can see how long this all takes
$timetmp = split(" ",microtime());
$timer_start_time = $timetmp[0] + $timetmp[1];

// set up the classes
$cache = new Cache;
$db_obj = new DB;


echo "sf-active ram_file regeneration script\n";

// select the necessary files
if($_SERVER['argv'][1] == "all")
{	
	$sql = "select linked_file, date_format(created,'%Y') as created_year, date_format(created, '%m') as created_month from webcast where mime_type = 'audio/x-pn-realaudio' or mime_type = 'video/x-pn-realvideo'" ;
}
elseif(is_numeric($_SERVER['argv'][1]) && is_numeric($_SERVER['argv'][2]))
{
	$sql = "select linked_file, date_format(created,'%Y') as created_year, date_format(created, '%m') as created_month from webcast where mime_type = 'audio/x-pn-realaudio' or mime_type = 'video/x-pn-realvideo' and id >= '";
	$sql .= $_SERVER['argv'][1]."' order by id asc limit 0,".$_SERVER['argv'][2];
}
else
{
	echo("to regenerate all ramfiles:\n");
	echo("/usr/bin/php cli_cache_ramfiles.php all\n");
	echo("to regenerate some ramfiles:\n");
	echo("usr/bin/php cli_cache_ramfiles.php [starting_id] [amount of files]\n");
	exit;
}
	
$result = $db_obj->query($sql);
while($row=array_pop($result))
{
	$upload_target = $row['linked_file'] ;
	$cache->cache_ram_file($upload_target, $row['created_year'], $row['created_month']);
	echo("creating ram_file for $upload_target\n");
}

$timetmp = split(" ",microtime());
$timer_finish_time = $timetmp[0] + $timetmp[1];

$timer_elapsed_time = $timer_finish_time - $timer_start_time;
$elapsed_in_seconds = sprintf("%0.5f", $timer_elapsed_time);

echo "Script completed in " . $elapsed_in_seconds . " seconds.\n";

?>
