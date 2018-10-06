<?php // vim:et:ai:ts=4:sw=4
namespace SFACTIVE\Calendar;

use SFACTIVE\DB;
use SFACTIVE\Tools;

include_once( SF_SHARED_PATH.'/classes/spamb_class.inc' );

class CalendarEventNotFoundException extends \Exception {
}

class Event extends \Cache {
	//The Event class is used to define an event in the calendar	
	
	var $id;
	var $title;
	var $start_time;
	var $end_time;
	var $start_day;
	var $end_day;
	var $start_month;
	var $end_month;
	var $start_year;
	var $end_year;
	var $start_day_of_week;
	var $description;
	var $detail_info_is_loaded=false;
	var $event_type_id;
	var $event_type_name;
	var $event_topic_id;
	var $event_topic_name;
	var $event_location_id;
	var $event_location_name;
	var $event_location_details;
	var $contact_name;
	var $contact_phone;
	var $contact_email;
  	var $confirmation_number;
	var $file ;
	var $mime_type;
	var $language_id;
	var $artmime;
	
	var $common_select_sql="Select e.event_id, e.title, e.confirmation_number, HOUR(e.start_date)+MINUTE(e.start_date)/60 as start_time, e.start_date, DAYOFWEEK(e.start_date) as start_day_of_week, DAYOFMONTH(e.start_date) as start_day, MONTH(e.start_date) as start_month, YEAR(e.start_date) as start_year, e.duration, e.location_id, e.location_other, e.event_topic_id, e.event_topic_other, e.event_type_id, e.event_type_other, l.name as location_name, eto.name as event_topic_name, ety.name as event_type_name, e.linked_file as linked_file, e.mime_type as mime_type, e.language_id as language_id, e.artmime as artmime from event e, location l, event_type ety, event_topic eto ";
	var $common_join_sql="e.event_type_id=ety.event_type_id and e.event_topic_id=eto.event_topic_id and e.location_id=l.location_id ";
	
	function Event(){
		//default constructor
	}
	
	function get_id(){
		//returns the id for the event
		return $this->id;
	}

	function set_id($new_id){
		//sets the id for the event
		$this->id=$new_id;
	}
	
	function get_title(){
		//returns the title for the event
		$title=$this->title;
		return Tools::saferHtml($title);
	}
	
	function set_title($new_title){
		//sets the title for the event
		$this->title=$new_title;
	}
	
	function get_start_time(){
		//returns the start time for an event
		return $this->start_time;
	}

	function set_start_time($new_start_time){
		//sets the start time for the event
		$this->start_time=$new_start_time;
	}

	function get_start_day(){
		//returns the day the event starts
		return $this->start_day;
	}

	function set_start_day($new_start_day){
		//sets the day teh event starts
		$this->start_day=$new_start_day;
	}
	
	function get_start_month(){
		//returns the month that the event starts
		return $this->start_month;
	}

	function set_start_month($new_start_month){
		//sets the month that the event starts
		$this->start_month=$new_start_month;
	}

	function get_start_year(){
		//returns the yeat that teh event starts
		return $this->start_year;
	}

	function set_start_year($new_start_year){
		//sets the year that the event starts
		$this->start_year=$new_start_year;
	}

	function get_start_day_of_week(){
		//returns the start day of the week that the event starts
		return $this->start_day_of_week;
	}

	function set_file($linked_file){
	    // sets the linked_file
	    return $this->file=$linked_file ;
	}

	function get_file(){
	    // returns the linked_file();
	    return $this->file ;
	}

	function set_mime_type($mime_type){
	    // sets the mime_type
	    return $this->mime_type=$mime_type;
	}

	function get_mime_type(){
	    // returns the mime_type
	    return $this->mime_type ;
	}

	function set_language_id($language_id){
	    // sets the language_id
	    return $this->language_id=$language_id ;
	}

	function get_language_id(){
	    // returns the language_id
	    return $this->language_id ;
	}

	function set_artmime($artmime){
	    // sets the mime_type of the text of the event (text or html)
	    return $this->artmime=$artmime;
	}

	function get_artmime(){
	    // returns the mime_type of the text of the event (either text or html)
	    return $this->artmime;
	}

	function set_start_day_of_week($new_start_day_of_week){
		//sets the start day of the week that teh event starts
		$this->start_day_of_week=$new_start_day_of_week;
	}

	function get_start_time_object(){
		//returns the start time that the event starts as a Time object
		$time= new \Time;
		$time->set_time($this->get_start_time());
		return $time;
	}

	function get_end_time_object(){
		//gets the time that the event ends
		$time=new \Time;
		$time->set_time($this->get_duration()+$this->get_start_time());
		return $time;
	}
	
	function get_start_date(){
		//returns the date and time that the event starts as a string
		$start_date=$this->get_start_year()."-";
		$start_date=$start_date.$this->get_start_month()."-";
		$start_date=$start_date.$this->get_start_day()." ";
		$start_time=$this->get_start_time();
		$start_hour=floor($start_time);
		$start_minute=round(60*($start_time-$start_hour));
		if ($start_minute<10){
			$start_minute="0".$start_minute;
		}
		if ($start_hour<10){
			$start_hour="0".$start_hour;
		}
		$start_date=$start_date.$start_hour.":".$start_minute.":00";
		return $start_date;
	}

	function get_start_date_object(){
		//returns the start date as a Date object
		$start_time=$this->get_start_time();
		$start_hour=floor($start_time);
		$start_minute=round(60*($start_time-$start_hour));
		$start_day=$this->get_start_day();
		$start_month=$this->get_start_month();
		$start_year=$this->get_start_year();
		$date= new \Date;
		$date->set_time($start_hour, $start_minute, $start_day, $start_month, $start_year);
		return $date;
	}
	
	function get_duration(){
		//returns the length of the event
		$duration=$this->duration;
		return Tools::saferHtml($duration);
	}

	function set_duration($new_duration){
		//sets the length of the event
		$this->duration=$new_duration;
	}
	
	function get_location_id(){
		//returns the location id for the event
		return $this->location_id;
	}
	
	function set_location_id($new_location_id){
		//sets the location id for the event
		$this->location_id=$new_location_id;
	}

	function get_location_name(){
		//get location name for the event (either other name or name from id)
		$location=$this->location_name;
		return Tools::saferHtml($location);
	}

	function set_location_other($new_location_other){
		//sets the "location other" field (name of a location not predefined in the DB)
		return $this->location_name=$new_location_other;
	}

	function get_location_details(){
		//returns details about the location for an event
		if ($this->detail_info_is_loaded==false){
			$this->load_detail_info();
		}
		$location_details=$this->location_details;
		return Tools::saferHtml($location_details);
	}

	function set_location_details($new_location_details){
		//sets details about the location for an event
		$this->detail_info_is_loaded=true;
		$this->location_details=$new_location_details;
	}
	
	function get_event_type_id(){
		//gets the event type id for an event
		return $this->event_type_id;
	}

	function set_event_type_id($new_event_type_id){
		//sets the event type id for an event
		$this->event_type_id=$new_event_type_id;
	}

	function get_event_type_name(){
		//returns the event type name for an event (either other value or name from id)
		$event_type_name=$this->event_type_name;
		return Tools::saferHtml($event_type_name);
	}

	function set_event_type_other($new_event_name){
		//sets the event type name for an event where the name isnt already in the DB
		$this->event_type_name=$new_event_name;
	}

	function get_event_topic_id(){
		//returns the event topic id for an event
		return $this->event_topic_id;
	}

	function set_event_topic_id($new_topic_id){
		//sets the event topic id for an event
		$this->event_topic_id=$new_topic_id;
	}

	function get_event_topic_name(){
		//returns the event topic name for an event (either other value of name from id)
		$topic=$this->event_topic_name;
		return Tools::saferHtml($topic);
	}

	function set_event_topic_other($new_event_topic_name){
		//sets the event topic name to be a nonstandard value (ie no id)
		$this->event_topic_name=$new_event_topic_name;
	}
	
	function get_contact_name(){
		//returns the contact name for the event
		if ($this->detail_info_is_loaded==false){
			$this->load_detail_info();
		}
		$contact_name=$this->contact_name;
		return Tools::saferHtml($contact_name);
	}

	function set_contact_name($new_contact_name){
		//sets the contact name for the event
		$this->detail_info_is_loaded=true;
		$this->contact_name=$new_contact_name;
	}

	function get_contact_phone(){
		//returns the contact phone number for an event
		if ($this->detail_info_is_loaded==false){
			$this->load_detail_info();
		}
		$contact_phone=$this->contact_phone;
		return Tools::saferHtml($contact_phone);
	}

	function set_contact_phone($new_contact_phone){
		//sets the conact phone number for an event
		$this->detail_info_is_loaded=true;
		$this->contact_phone=$new_contact_phone;
	}

	function get_contact_email(){
		//returns the contact email address for an event
		if ($this->detail_info_is_loaded==false){
			$this->load_detail_info();
		}
		$contact_email=$this->contact_email;
		return Tools::saferHtml($contact_email);
	}

	function set_contact_email($new_contact_email){
		//sets the contact email address for an event
		$this->detail_info_is_loaded=true;
		$this->contact_email=$new_contact_email;
	}
	
	function get_description(){
		//returns a description for an event
		if ($this->detail_info_is_loaded==false){
			$this->load_detail_info();
		}
		$descr=$this->description;
		return Tools::saferHtml($descr);
	}

	function set_description($new_description){
		//sets the description for an event
		$this->description=$new_description;
		$this->detail_info_is_loaded=true;
	}
	
	function get_confirmation_number(){
		//return the confirmation number used for updating and event
		return $this->confirmation_number;
	}

	function set_confirmation_number($new_conf_num){
		$this->confirmation_number=$new_conf_num;
	}

	function get_weekname(){
		//returns the name fo teh week for an event (used for the caced week file)
		$date= new \Date;
		$date->set_time(0,0, $this->get_start_day(), $this->get_start_month(), $this->get_start_year());
		return $date->get_weekname();
	}	
	function parse_db_row($db_row){
        $this->rawData = array_merge([], $db_row);
		$this->set_id($db_row["event_id"]);
		$this->set_title($db_row["title"]);
		$this->set_start_time($db_row["start_time"]);
		$this->set_start_day($db_row["start_day"]);
		$this->set_start_month($db_row["start_month"]);
		$this->set_start_year($db_row["start_year"]);
		$this->set_start_day_of_week($db_row["start_day_of_week"]-1);
		$this->set_duration($db_row["duration"]);
		$this->set_event_type_id($db_row["event_type_id"]);
		if ($db_row["event_type_id"]==0){
			$this->set_event_type_other($db_row["event_type_other"]);
		}else{
			$this->set_event_type_other($db_row["event_type_name"]);
		}
		$this->set_event_topic_id($db_row["event_topic_id"]);
		if ($db_row["event_topic_id"]==0){
			$this->set_event_topic_other($db_row["event_topic_other"]);
		}else{
			$this->set_event_topic_other($db_row["event_topic_name"]);
		}
		$this->set_location_id($db_row["location_id"]);
		if ($db_row["location_id"]==0){
			$this->set_location_other($db_row["location_other"]);
		}else{
			$this->set_location_other($db_row["location_name"]);
		}
		$this->set_confirmation_number($db_row["confirmation_number"]);
		if(strlen($db_row['linked_file']) > 0)
		{
		    $this->set_file($db_row['linked_file']);
		}
		if(strlen($db_row['mime_type']) > 0)
		{
		    $this->set_mime_type($db_row['mime_type']);
		}
		$this->set_language_id($db_row['language_id']);
		$this->set_artmime($db_row['artmime']);
	}


	function load_detail_info(){
		//loads event information not normally needed for lists of events
		global $db_obj;
		$sql="select description, contact_name, contact_phone, ";
		$sql=$sql."contact_email, location_details from event where event_id=";
		$sql=$sql.$this->get_id();
		$resultset=$db_obj->query($sql);
		if (sizeof($resultset)==1){
			$result_row=$resultset[0];
			$this->set_description($result_row["description"]);
			$this->set_location_details($result_row["location_details"]);
			$this->set_contact_name($result_row["contact_name"]);
			$this->set_contact_phone($result_row["contact_phone"]);
			$this->set_contact_email($result_row["contact_email"]);
			$this->detail_info_is_loaded=true;
		}
	}	

	/**
 	 * Checks the title, description for spam.
	 */
	function _check_spam() {
		$spam = new \SpamB();
		$userIP = $_SERVER['REMOTE_ADDR'];  // eventually, should add this to the object
		$this->spam = (
			$spam->detect_ip( $userIP ) ||
			$spam->detect_strings( $this->get_description() ) ||
			$spam->detect_strings( $this->get_title() ) ||
			$spam->too_many_urls( $this->get_description() )
		);
	}

	function add(){
		//add this event to the db (fields must already be set)

		// check for spam.  don't allow spam.
		$this->_check_spam();
		if ($this->spam)
			die();

		$db_obj = new DB();

		srand((double)microtime()*1000);
		$this->confirmation_number = rand(1000,9999).rand(1000,9999);

		$sql ="INSERT INTO event 
               (title, start_date, duration, location_id, location_other, location_details,
		       event_type_id, event_type_other, event_topic_id, event_topic_other, contact_name, contact_phone, 
               contact_email, confirmation_number, description, linked_file, mime_type, language_id, artmime)
		       VALUES 
               (:title, :start_date, :duration, :location_id, :location_other, :location_details,
		       :event_type_id, :event_type_other, :event_topic_id, :event_topic_other, :contact_name, :contact_phone, 
               :contact_email, :confirmation_number, :description, :linked_file, :mime_type, :language_id, :artmime)
               ";

		$values = $this->asQueryParameterValues();
        unset($values[':event_id']);
		$result=$db_obj->insert($sql, $values);
		$this->set_id($result);

		if ($result<0){
			return $result;
		}
		
		return $this->confirmation_number;
	}

    function asAssociativeArray() 
    {
        return [
		 'event_id' => $this->get_id(),
		 'title' => $this->get_title(),
		 'start_date' => $this->get_start_date(),
		 'duration' => $this->get_duration(),
		 'location_id' => $this->get_location_id(),
		 'location_other' => $this->get_location_name(),
		 'location_details' => $this->get_location_details(),
		 'event_type_id' => $this->get_event_type_id(),
		 'event_type_other' => $this->get_event_type_name(),
		 'event_topic_id' => $this->get_event_topic_id(),
		 'event_topic_other' => $this->get_event_topic_name(),
		 'contact_name' => $this->get_contact_name(),
		 'contact_phone' => $this->get_contact_phone(),
		 'contact_email' => $this->get_contact_email(),
		 'confirmation_number' => $this->confirmation_number,
		 'description' => $this->get_description(),
		 'linked_file' => $this->get_file(),
		 'mime_type' => $this->get_mime_type(),
		 'language_id' => $this->get_language_id(),
		 'artmime' => $this->get_artmime()
         ];
    }

    function asQueryParameterValues() 
    {
        $b = [];
        foreach($this->asAssociativeArray() as $key=>$value) {
            $b[':'.$key] = $value;
        }
        return $b;
    }

	function update($confirmation_number = null){
		//updates the DB to have the values in this object (takes teh confirmation
		//number to validate user identification)
        $db_obj = new \SFACTIVE\DB;
		if (! filter_var( $confirmation_number, FILTER_VALIDATE_INT )) {
			return -1;
		}
		$sql="select event_id from event where event_id=".$this->get_id();
		$sql=$sql." and confirmation_number=".$confirmation_number;
		$test_result=$db_obj->query($sql);
		if (sizeof($test_result)>0){

            $sql = "UPDATE EVENT SET title=:title, 
                    start_date=:start_date,
                    duration=:duration,
                    location_id=:location_id,
                    location_other=:location_other,
                    event_topic_id=:event_topic_id,
                    event_topic_other=:event_topic_other,
                    event_type_id=:event_type_id,
                    event_type_other=:event_type_other,
                    location_details=:location_details,
                    contact_name=:contact_name,
                    contact_phone=:contact_phone,
                    contact_email=:contact_email,
                    description=:description,
                    linked_file=:linked_file,
                    mime_type=:mime_type,
                    language_id=:language_id,
                    artmime=:artmime,
                    WHERE event_id=:event_id";

            $values = $this->asQueryParameterValues();
            unset($values[':confirmation_number']);

			$return_val=$db_obj->update($sql, $values);
		}else{
			$return_val=-1;
		}
		return $return_val;
	}
	
	function delete($confirmation_number){
        $db_obj = new \SFACTIVE\DB;
		if (! filter_var( $confirmation_number, FILTER_VALIDATE_INT )) {
			return -1;
		}
		$sql="SELECT event_id FROM event where event_id=".$this->get_id();
		$sql=$sql." AND confirmation_number=".$confirmation_number;
		$test_result=$db_obj->query($sql);
		if (sizeof($test_result)>0){
			$sql="DELETE FROM event WHERE event_id=".$this->get_id();
			$return_val=$db_obj->execute($sql);
		}else{
			$return_val=-1;
		}
		return $return_val;
	}
	
	// johnk - what the f is this function???
	function find_by_id($event_id){
		//loads the values of an event into a new Event object given an id
		global $db_obj;
		$db_obj = new DB();
		$probe = $db_obj->query("select count(*) as c from event where event_id=".$event_id);
		if ($probe[0]['c']==0) {
			throw new CalendarEventNotFoundException("Event not found for $event_id");
		}

		$sql=$this->common_select_sql." where e.event_id=";
		$sql=$sql.$event_id;
		$sql=$sql." AND ".$this->common_join_sql;
		$events=$db_obj->query($sql);
		if (sizeof($events)>0){
			$new_event=new Event();
			$next_db_row=$events[0];
			$new_event->parse_db_row($next_db_row);
			return $new_event;
		} else {
			throw new CalendarEventNotFoundException("Event not found for $event_id");
		}
	}

	function load_by_id($event_id) {
		global $db_obj;
		if (!$event_id) throw new CalendarEventNotFoundException("Invalid event_id parameter.");

		$db_obj = new DB();
		$probe = $db_obj->query("select count(*) as c from event where event_id=".$event_id);
		if ($probe[0]['c']==0) {
			throw new CalendarEventNotFoundException("Event not found for $event_id");
		}

		$sql=$this->common_select_sql." where e.event_id=";
		$sql=$sql.$event_id;
		$sql=$sql." AND ".$this->common_join_sql;
		$events=$db_obj->query($sql);
		if (sizeof($events)>0){
			$next_db_row=$events[0];
			$this->parse_db_row($next_db_row);
		} else {
			throw new CalendarEventNotFoundException("Event not found for $event_id");
		}
	}

	function find_between_dates($begin_date, $end_date, $filter, $limit=0){
		//returns a list of events given a date range and a filter of field names and values

		global $db_obj;
		$db_obj = new DB();
		
		$sql=$this->common_select_sql;
		$sql=$sql." where  e.start_date > \"".$begin_date."\"";
		$sql=$sql." and e.start_date < \"".$end_date."\"";
		if (strlen($filter['topic_id'])>0){
			$sql=$sql." and e.event_topic_id=".$filter['topic_id'];
		}
		if (strlen($filter['type_id'])>0){
			$sql=$sql." and e.event_type_id=".$filter['type_id'];
		}
		if (strlen($filter['location_id'])>0){
			$sql=$sql." and e.location_id=".$filter['location_id'];
		}
		$sql=$sql." and ".$this->common_join_sql;
		$sql=$sql." order by e.start_date";
		if ($limit != 0 && $limit > 0) {
		    $sql .= ' limit ' . $limit;
		}
		$result=$db_obj->query($sql);
		$i=0;
		$day=0;
		$return_array=array();
		if (sizeof($result)>0 ){
			while ($next_event_row=array_shift($result)){
				$next_event=new Event();
				$next_event->parse_db_row($next_event_row);
				array_push($return_array, $next_event);
			}
		}	
		return $return_array;
	}

	function find_all(){
		//returns all events in the system from the DB

        $db_obj = new \SFACTIVE\DB;
		$sql=$this->common_select_sql;
		$sql=$sql." where ".$this->common_join_sql;
		$sql=$sql." order by e.start_date";
		$result=$db_obj->query($sql);
		$i=0;
		$day=0;
		$return_array=array();
		if (sizeof($result)>0){
			while ($next_event_row=array_shift($result)){
				$next_event=new Event();
				$next_event->parse_db_row($next_event_row);
				array_push($return_array, $next_event);
			}
		}	
		return $return_array;
	}
	
	function createWeekCache ($newdate) {
        // Generates the Cache Page HTML for a Week
		$ccm = new \SFACTIVE\Calendar\CalendarCacheManager(SF_CACHE_PATH.'/calendar');
		$newdate->find_start_of_week();
		$ccm->cache_week($newdate);
	}

	function createEventCache ($eventid) {
        // Generates the Cache Page HTML for a Event
		$ccm = new \SFACTIVE\Calendar\CalendarCacheManager(SF_CACHE_PATH.'/calendar');
		$ccm->cache_event($eventid);
	}

	function get_syndication_data ($location)
        {
            // gets data out of the dbase needed for creating rss files.
            if(strlen($location) > 0)
            {
                $location = " and e.location_id = '".$location."' ";
            }
            $sql = "select e.event_id as event_id, date_format(e.start_date, '%Y-%m-%d %H:%i:%s') as start_date, date_format((e.start_date + interval e.duration hour), '%Y-%m-%d %H:%i:%s') as end_date, e.title as title, e.description as description, e.contact_name as contact_name, e.event_topic_other as topic_other, e.location_other as location_other, e.event_type_other as type_other, l.name as location_name, t.name as type_name, o.name as topic_name, e.linked_file as linked_file, e.language_id as language_id from event e, location l, event_type t, event_topic o where e.start_date > CURRENT_TIMESTAMP ".$location." and e.location_id = l.location_id and e.event_type_id = t.event_type_id and e.event_topic_id = o.event_topic_id order by e.start_date asc limit 0,".$GLOBALS['calendar_length'] ;
            $db_obj = new DB ;
            $result = $db_obj->query($sql);
            return $result ;
        }


    function make_calendar_rdf ($location="")
    {
	$GLOBALS['print_rss'] = "" ;
	$lang_obj = new \language;
	$syndication_data=$this->get_syndication_data($location);
	$rows = sizeof($syndication_data);
	$time_zone = date('O');
	$webroot_url = SF_ROOT_URL;
	$site_nick = $GLOBALS['site_nick'];
	$site_name = $GLOBALS['site_name'];
	$rss_file = $GLOBALS['calendar_file'] ;
        $xml_logo = $GLOBALS['xml_logo'];
        include_once(SF_SHARED_PATH . '/classes/rss10.inc');
	if($location !== "") { $location = strtolower(str_replace(" ", "_", $syndication_data['0']['location_name']))."_"; }
	$rss_long=new \RSSWriter($webroot_url, $site_nick, $site_name, $webroot_url."/syn/".$location.$rss_file, array("dc:publisher" => $site_nick, "dc:creator" => $site_nick));
	$rss_long->useModule("ev", "http://purl.org/rss/1.0/modules/event/");
	$rss_long->useModule("content", "http://purl.org/rss/1.0/modules/content/");
        $rss_long->useModule("dcterms", "http://purl.org/dc/terms/");
        $rss_long->setImage($xml_logo, $site_nick);
	foreach (array_reverse($syndication_data) as $row)
        {
	    if($GLOBALS['lang'] == "hr") { $char = "utf-8" ; }
	    else { $char = "cp1252";}
	    $this_feature_url = "/calendar/event_display_detail.php?event_id=" . $row['event_id'];
	    $link = $webroot_url . $this_feature_url;
            if(strpos($row['linked_file'], SF_UPLOAD_PATH)==0)
            {
                // note: this will only work if you never moved your images across the filesystem.
                $file = str_replace(SF_UPLOAD_PATH, SF_UPLOAD_URL, $row['linked_file']) ;
            }else {
                $file = '';
            }
	    $author = trim(utf8_encode(htmlspecialchars($row['contact_name'])));
	    $title1 = trim(utf8_encode(htmlspecialchars($row['title'])));
	    $row['description_new'] = preg_replace("(<.*?>)", '', $row['description']);
	    $title2 = trim(utf8_encode(htmlspecialchars($row['description_new'])));
	    $event_topic = trim(utf8_encode(htmlspecialchars($row['topic_name'])));
	    if($row["topic_other"] !=="") { $event_topic .= " (".trim(utf8_encode(htmlspecialchars($row['topic_other']))).")"; }
	    $event_location = trim(utf8_encode(htmlspecialchars($row['location_name'])));
	    if($row["location_other"] !=="") { $event_location .= " (". trim(utf8_encode(htmlspecialchars($row['location_other']))). ")"; }
	    $event_type = trim(utf8_encode(htmlspecialchars($row['type_name'])));
	    if($row["type_other"] !=="") { $event_type .= " (".trim(utf8_encode(htmlspecialchars($row['type_other']))).")"; }
	    $cat_name = trim(utf8_encode(htmlspecialchars($row['name'])));
	    $subject = trim(utf8_encode(htmlspecialchars($row['topic_id'])));
	    $summary = str_replace(array('&lt;','&gt;','&amp;', '&quot;'), array('<','>','&', '"'), htmlentities(stripslashes($row['description']), ENT_QUOTES, $char));
	    $date = gmdate('Y-m-d\TH:i:s\Z',strtotime($row['start_date']));
	    $enddate = gmdate('Y-m-d\TH:i:s\Z',strtotime($row['end_date']));
	    $language = trim(utf8_encode($lang_obj->get_language_code($row['language_id'])));
	    $rss_long->addItem($link, $title1, array("description" => $title2, "dc:date" => $date, "dc:subject" => $event_topic, "dc:creator" => $author, "dc:resource" => "event", "dc:language" => $language, "ev:startdate" => $date, "ev:type" => $event_type, "ev:location" => $event_location, "ev:enddate" => $enddate, "content:encoded" => $summary, "dcterms:hasPart" => $file));
	}
        $print_long=$rss_long->serialize();
	$print_long=str_replace(array("&amp;#","&amp;amp;","&amp;gt;","&amp;lt;","&amp;quot;"), array("&#", "&amp;", "&gt;", "&lt;", "&quot;"), $print_long);
	$fffp = fopen(SF_WEB_PATH."/syn/".$location.$rss_file, "w");
        fwrite($fffp, $print_long, strlen($print_long));
        fclose($fffp);
        }

    function set_this_delete_info($event_id)
    {
	$db = new DB;
	$query = "select confirmation_number, location_id from event where event_id = '".$event_id."'";
	$result = $db->query($query);
	while($row = array_pop($result))
	{
	    $this->delete_info["location_id"] = $row['location_id'] ;
	    $this->delete_info["confirmation_number"] = $row['confirmation_number'] ;
	}
	return $this->delete_info;
    }

    function make_calendar_minical($location_id = '') {
	include_once(SF_SHARED_PATH.'/classes/calendar/minical.inc');
	if($location_id > 0) $loc_array['location_id'] = $location_id ;
	$minical = new Minical($loc_array);
	$minical->cache();
    }

    function update_from_yesterday()
    {
	// if there was an event yesterday, we need to update minicalendar.html & calendar.rdf
	// if the events of yesterday had a specific location set, we need to update those too.
	$db = new DB ;
	$yesterday = date('U', time()-86400);
	$query = "select event_id, location_id from event where event.start_date like '".date('Y-m-d', $yesterday)."%'";
	$result = $db->query($query);
	if(is_array($result) && sizeof($result) > 0)
	{
	    $this->make_calendar_rdf();
	    $this->make_calendar_minical();
	    $locs = array();
	    while($row = array_pop($result))
	    {
		if($row['location_id'] && !in_array($row['location_id'], $locs))
		{
		    $locs[] = $row['location_id'] ;
		    $this->make_calendar_rdf($row['location_id']);
		    $this->make_calendar_minical($row['location_id']);
		}
	    }
	    return 1;
	}
    }

}
