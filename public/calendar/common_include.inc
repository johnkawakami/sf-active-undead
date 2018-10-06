<?php
//this is the common set of includes needed by calendar pages
/*
CalendarCacheManager.php  Event.php*                EventRenderer.php*        EventType.php*            Location.php*
DBBasedObject.php*        EventDisplay.php*         EventTopic.php*           EventWeek.php*            Minical.php 
*/

	include_once SF_CLASS_PATH."/calendar/calendar.inc";
	include_once SF_CLASS_PATH."/calendar/indycalendar.inc";	
	include_once SF_CLASS_PATH."/calendar/default_object.inc";
	// include_once SF_CLASS_PATH."/calendar/db_based_object.inc";
	include_once SF_CLASS_PATH."/calendar/event_renderer.inc";
	// include_once SF_CLASS_PATH."/calendar/event_display.inc";
	// include_once SF_CLASS_PATH."/calendar/event_topic.inc";
	// include_once SF_CLASS_PATH."/calendar/event_type.inc";
	// include_once SF_CLASS_PATH."/calendar/location.inc";
	// include_once SF_CLASS_PATH."/calendar/event.inc";
	include_once SF_CLASS_PATH."/calendar/date_renderer.inc";
	include_once SF_CLASS_PATH."/time.inc";
	// include_once SF_CLASS_PATH."/calendar/event_week.inc";
	include_once SF_CACHE_PATH."/location_options.inc";
	include_once SF_CACHE_PATH."/event_topic_options.inc";
	include_once SF_CACHE_PATH."/event_type_options.inc";
	include_once SF_CLASS_PATH."/calendar/calendar_cache_manager.inc";
	include_once SF_CLASS_PATH."/calendar/utilities.inc";

	$GLOBALS['calendar_cache_manager'] = new CalendarCacheManager(SF_CACHE_PATH.'/calendar/');
	$GLOBALS['calendar_cache_manager7'] = new \SFACTIVE\Calendar\CalendarCacheManager(SF_CACHE_PATH.'/calendar/');

?>
