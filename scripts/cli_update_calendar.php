<?php

/*
 * usage: php cli_update_calendar.php
 *
 * This script should be run out of cron somewhere after midnight.
 * This script will check which entries your calendar had for yesterday
 * if there were events yesterday, we'll update the minical & rdf feeds.
 *
 * NOTE: this script will probably run under your username.
 * you'll have to make sure you can write to the minical.html & calendar.rdf files.
 * if you can't you should either run this under the user of your webserver or ask
 * your sysadmin to set the permissions ok.
 *
 */

// you need to edit the following settings in order to make it work.
ini_set('include_path', '.:/www/sf-active:');
$_SERVER['SITE_NAME'] = 'sitename' ;
include('shared/global.cfg');

// let's include what we need.
include_once(SF_CALENDAR_URL.'/common_include.inc');
include_once(SF_CLASS_PATH.'/calendar/minical.inc');

// all we need is event->update_from_yesterday();
$event = new Event ;
$event->update_from_yesterday() ; 

?>
