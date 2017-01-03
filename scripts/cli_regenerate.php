<?php

// this script is used to regenerate articles from the command line for sf-active
// it should be run from local/scripts/ so that it can correctly find the sfactive.cfg file
// note that local/scripts/ usually does not exist :)
// syntax is /usr/local/bin/php cli_regenerate.php [starting id number] [number to generate]
//
// *WARNING*: you'll need write permissions in both SF_NEWS_PATH & SF_UPLOAD_PATH (articles & uploads)
//
// written by gek, feb 1 2003

set_time_limit(0);
ini_set('display_errors', true);
ini_set('include_path', '.:/usr/local/sf-active');

// Set up a timer so we can see how long this all takes
$timetmp = split(" ",microtime());
$timer_start_time = $timetmp[0] + $timetmp[1];

// Get the parent directory name so we can find sfactive.cfg
$path = dirname(dirname(realpath(__FILE__)));

include_once("$path/local/config/sfactive.cfg");

// Get variables and set script vars
$start_id = $_SERVER['argv'][1];
$num_gen  = $_SERVER['argv'][2];

$mode_regen = false;
$mode_update = "Not regenerating articles, displaying stats only.";

// Check if we are actually regenerating or just displaying stats
if (is_numeric($start_id))
{
    if ($start_id > 0)
    {
        $mode_regen = true;
        $mode_update = "Regenerating articles starting with id # " . $start_id;

        if (is_numeric($num_gen))
        {
            if ($num_gen > 0)
            {
                $mode_update .= ", regenerating " . $num_gen . " articles";
            } else
            {
                $mode_update .= ", regenerating all articles";
            }
        }
    }
}

echo "sf-active webcast regenerate script\n";
echo $mode_update . "\n";
echo "Command line: " . join(' ', $_SERVER['argv']) . "\n\n";
//echo "Path: " . $path . "\n\n";

echo "Trying to get database connection: ";
$db = new DB;
echo "OK.\n";

echo "Getting general webcast statistics: ";
/*
$query = "SELECT date_format(min(created), '%M %d %Y'), date_format(max(created), '%M %d %Y'),
                 to_days(now()) - to_days(min(created)) 
          FROM   webcast";

$db_result = $db->query($query);

$sfa_dayinfo = array_pop($db_result);

$query = "SELECT count(*)
          FROM   webcast
          WHERE  parent_id = 0
            AND  arttype = 'webcast'
          ";

$db_result = $db->query($query);

$sfa_artnum = array_pop($db_result);

$query = "SELECT count(*)
          FROM   webcast
          WHERE  parent_id != 0
            AND  arttype = 'news-response'
          ";

$db_result = $db->query($query);
$sfa_artcomm = array_pop($db_result);

$query = "SELECT count(*)
          FROM   webcast
          WHERE  parent_id = 0
            AND  arttype = 'webcast'
            AND  display = 'f'
         ";

$db_result = $db->query($query);
$sfa_hidden = array_pop($db_result);
echo "OK.\n\n";

echo "First article posted on: " . $sfa_dayinfo[0] . "\n";
echo "Latest article posted on: " . $sfa_dayinfo[1]  . "\n";
echo "Total articles posted: " . number_format($sfa_artnum[0]) . "\n";
echo "Total hidden articles: " . number_format($sfa_hidden[0]) . "\n";
echo "Total comments posted: " . number_format($sfa_artcomm[0]) . "\n";
echo "Average articles per day: " . number_format($sfa_artnum[0]/$sfa_dayinfo[2]) . "\n";
echo "Average comments per day: " . number_format($sfa_artcomm[0]/$sfa_dayinfo[2]) . "\n\n";
*/
if ($mode_regen === true)
{
    echo "Initializing article regeneration: ";
    $i = 0;
    $article_obj = new Article();
    $article_list = $article_obj->get_post_ids_starting_with($start_id, $num_gen);
    echo "OK.\n\n";

    while (($next_row = array_pop($article_list)))
    {
        $id = $next_row['id'];

        echo "Writing article $id: ";
	$GLOBALS['move_attachments'] = '1';
        $article = new Article($id);
	$article->handle_move_uploads();
        $article->render_everything();
        $article->cache_to_disk();
        echo "OK.\n";
    }
}

$timetmp = split(" ",microtime());
$timer_finish_time = $timetmp[0] + $timetmp[1];

$timer_elapsed_time = $timer_finish_time - $timer_start_time;
$elapsed_in_seconds = sprintf("%0.5f", $timer_elapsed_time);

echo "Script completed in " . $elapsed_in_seconds . " seconds.\n";

?>
