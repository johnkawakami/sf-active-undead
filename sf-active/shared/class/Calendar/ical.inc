<?php
/**
 * Creates a vcal version 1.0 stream from an sf-active event object
 *
 *  @package	calendar
 */

class iCal {

	/**
	 *  array of Sf-Active Event objects
	 *  @var    array
	 */
	var $events;
	/**
	 *  @var    string
	 */
	var $timezone;
	/**
	 *  @var    string
	 */
	var $prodid;

	/**
	 *  class constructor. sets the default values.
	 *  @param  array   $events_array
	 *  @param  string  $prodid
	 *  @param  string  $timezone
	 */
	function iCal($events_array = Array(), $prodid = 'SF-Active Web Calendar', $timezone = '-0700') {
		$this->prodid = $prodid;
		$this->timezone = $timezone;
		$this->events = $events_array;
	}

	/**
	 *  takes sf-active Event object
	 *  @param  object  $event
	 */
	function addEvent($event) {
		$this->events[] = $event;
	}

	/**
	 *  either vcal or ical type output
	 *  @param  string  $type
	 *  @return string
	 */
	function toString($type = 'v' ) {
		return $type == 'v' ? $this->display_vcal() : $this->display_ical(); 
	}


	/**
	 * Alias of toString()
	 */
	function display() {
		return $this->toString();
	}

	/**
	 *  renders as vcal.
	 * @returns a vcal stream
	 */
	function display_vcal() {
		$text = "BEGIN:VCALENDAR\r\nVERSION:1.0\r\nPRODID:{$this->prodid}\r\nTZ:{$this->timezone}";
		foreach ($this->events as $event) {
			$text .= "\r\n" . $this->event_to_vcal($event);
		}
		$text .= "\r\nEND:VCALENDAR";
		return $text;
	}

	/**
	 *  renders as ical.
	 *  @return string
	 */
	function display_ical() {
		$text = "BEGIN:VCALENDAR\r\nVERSION:2.0\r\nPRODID:{$this->prodid}\r\nTZ:{$this->timezone}";
		foreach ($this->events as $event) {
			$text .= "\r\n" . $this->event_to_vcal($event);
		}
		$text .= "\r\nEND:VCALENDAR";
		return $text;
	}
	
	/**
	 * cleanup a string to remove characters not allowed in a 
	 * vcal format file
	 * taken from RSSiCal version 0.8.3
	 *  @param  string  $ts
	 */
	function cleanup($ts) {

		$patterns = array(
			'/<br \/>/',
			'/<\/p>/',
			'/<p.*?>/',
			
			'/(.*?)<a href="(.*?http:\/\/.+?)".*?>(.*?)<\/a>(.*?)/',			
			
			'/<code.*?>(.*?)<\/code>/',
			'/<img.*?src=".*?".*?[\/]*>/',		
			
			'/<blockquote.*?cite="(http:\/\/.+?)".*?>(.*?)<\/blockquote>/',	
			'/<cite.*?>(.*?)<\/cite>/',
			
			'/<[ou]l*?>(.+?)<\/[ou]l>/',
			'/<li*?>(.+?)<\/li>/',

			
			'/<span.*?>(.*?)<\/span>/',
			'/<strong.*?>(.*?)<\/strong>/',
			'/<b.*?>(.*?)<\/b>/',
			'/<em.*?>(.*?)<\/em>/',
			'/<i.*?>(.*?)<\/i>/',
			'/,/',
			
			'/&lt;/',
			'/&gt;/',
			'/&amp;/',
			'/&#821[67];/',
			'/&#822[01];/'
			);
			
		$replace = array(
			//br and p	
			"\\n",
			"\\n\\n",
			"",
			//a element			
			"\\1\\3 [link: \\2]\\4",
			//code
			"\\1",
			//img
			"[img]\\n",
			//cite=,cite	
			"\\n\\2[cite: \\1]\\n\\n",			
			"[cite]\\1",
			//ul/ol, li
			"\\n\\1",
			"* \\1\\n",

			"\\1",
			"\\1",
			"\\1",
			"\\1",
			"\\1",
			"\\,",
			
			"<",
			">",
			"&",
			"'",
			"\""
			);
			
		$ts = preg_replace($patterns, $replace, $ts);
		return $ts;
	}
	
	/**
	 *  renders an event into vcal format.
	 *  @param  object  $event
	 *  @return string
	 */
	function event_to_vcal($event) {
		
		$dtstart = date("Ymd\THi00", strtotime($event->get_start_date()));
		$dtend = date("Ymd\THi00", strtotime($event->get_start_date()) + ($event->get_duration() * 60 * 60));
		$description =  $this->cleanup($event->get_description());
//		$description = $event->get_description();
		$location_details = $this->cleanup($event->get_location_details());
		$categories = strtoupper($event->get_event_topic_name() . ';' . $event->get_event_type_name());
	
		$text = 'BEGIN:VEVENT';
		$text .= "\r\nUID:". $event->get_confirmation_number() . $event->get_id();
		$text .= "\r\nCATEGORIES:" . $categories; 
//		$text .= "\r\nCATEGORIES:INDYMEDIA";
		$text .= "\r\nSUMMARY:" . $event->get_title();
		$text .= "\r\nDESCRIPTION: "
			. $description . "\r\n  =0D=0A= Contact: " . $event->get_contact_name() . ' <' . $event->get_contact_email() . '> ' . $event->get_contact_phone();
		$text .= "\r\nLOCATION:" . $event->get_location_name() 
			. '(' . $get_location_details . ')';
		$text .= "\r\nATTENDEE;ROLE=OWNER;STATUS=CONFIRMED: "
			. '"' . $event->get_contact_name() . '" <' . $event->get_contact_email() . '>';
		$text .= "\r\nCLASS:PUBLIC";
		$text .= "\r\nDTSTART:$dtstart";
		$text .= "\r\nDTEND:$dtend";
		$text .= "\r\nEND:VEVENT";

		return $text;
	}
}

?>
