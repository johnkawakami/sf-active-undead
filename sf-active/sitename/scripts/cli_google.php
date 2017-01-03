<?php

// this script is used to regenerate articles from the command line for sf-active
// it should be run from local/scripts/ so that it can correctly find the sfactive.cfg file
// note that local/scripts/ usually does not exist :)
// syntax is /usr/local/bin/php cli_regenerate.php [starting id number] [number to generate]
// written by gek, feb 1 2003

ini_set('display_errors', true);
ini_set('include_path', '.:/www/sf-active');

// Set up a timer so we can see how long this all takes
$timetmp = split(" ",microtime());
$timer_start_time = $timetmp[0] + $timetmp[1];

// Get the parent directory name so we can find sfactive.cfg
$path = dirname(dirname(realpath(__FILE__)));

include_once("$path/local/config/sfactive.cfg");

// Get variables and set script vars

$out_file = SF_WEB_PATH . '/google.html';
$summaries_file = SF_CACHE_PATH . '/summaries.html';
$center_file    = SF_CACHE_PATH . '/center_column_production.html';
$output = "";

$output .= "<HTML><HEAD><TITLE>SF Indymedia News for Google</TITLE></HEAD><BODY>\n";
$output .= "<BR>This page is set up for Google News crawler. All links on this page have ";
$output .= "been reviewed by a volunteer group of editors who are accountable to the larger Indymedia ";
$output .= "organization. If you have any questions, please email sf@indymedia.org\n<BR><BR>";

$fh = fopen($summaries_file, "r");

$pattern = "Other/Breaking News";
$pattern2 = "^<a";

while (!feof($fh))
{
    $getline = fgets($fh, 4096);
    if (ereg($pattern, $getline)) break;
    if (ereg($pattern2, $getline)) $output .= $getline . "<BR><BR>\n\n";
}

fclose($fh);

$fh = fopen($center_file, "r");

while (!feof($fh))
{
    $getline = fgets($fh, 4096);
    $output .= $getline;
}

fclose($fh);

$output .= "<BR><BR><BR>\n\n\n</BODY></HTML>";

$fh = fopen($out_file, "w");
fwrite ($fh, $output);
fclose($fh);

?>
