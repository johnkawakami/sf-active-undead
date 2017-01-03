<? 
$timetmp = split(" ",microtime());
$timer_start_time = $timetmp[0] + $timetmp[1];

ini_set('include_path', '.:/www/sf-active/');

// $_ENV['SITE_NAME'] = "sitename";
$_SERVER['SITE_NAME'] = "sitename" ;
include_once("shared/global.cfg");
include(SF_CLASS_PATH.'/gallery_class.inc');

if($argv['1'] == "all")
{
    $x = new Gallery('') ;
    $x->render_all_pages();
    $disp_time = "1";
}
elseif($argv[1] == "now")
{
    $x = new Gallery('');
    $x->render_this_month();
    $disp_time = "1";
}
elseif($argv['1'] && $argv['2'] && $argv['3'])
{
    $x = new Gallery($argv[1], $argv[2], "$argv[3]");
    $disp_time = "1";
}else{
    echo("usage: all galleries:  /usr/bin/php create_galleries.php all\n");
    echo("usage: this month: /usr/bin/php create_galleries.php now\n");
    echo("or: certain month: /usr/bin/php create_galleries.php category_id year month\n\n");
}


$timetmp = split(" ",microtime());
$timer_finish_time = $timetmp[0] + $timetmp[1];

$timer_elapsed_time = $timer_finish_time - $timer_start_time;
$elapsed_in_seconds = sprintf("%0.5f", $timer_elapsed_time);

if($disp_time) { echo "Script completed in " . $elapsed_in_seconds . " seconds.\n"; }


?>
