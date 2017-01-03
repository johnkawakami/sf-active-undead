<?php

class EventJson extends Event 
{

	function __construct( $id ) 
	{
		$this->event = new Event();
		$this->event->find_by_id( $id );
	}
	/**
	 * The format is json, but the field names
	 * are from .ics files, but without "v" in the name.
	 * Some values are in the ics format.  It's a really ugly
	 * "frankenformat".
	 * http://www.ietf.org/rfc/rfc2445.txt
	 */
	function asJSON() 
	{
		$event = array(
			'DTSTART' => 'TZID=America/Los_Angeles;YYYYMMDDTHHMMSS',
			'DURATION' => 'P2H30M',
			'SUMMARY' => $title,
			'DESCRIPTION' => $description,
			'CATEGORIES' => array( $topic, $type, $location ),
			'GEO' => '37.386013;-122.082932',
			'LOCATION' => '',
			'CONTACT' => 'ALTREP="http://url":Joe Blow, 555-1212'
		);
		$attachments = array(
			array( 
				'NAME' => 'Flyer Image',
				'TYPE' => 'image/jpeg',
				'URI' => ''
			),
			array(
				'NAME' => 'Printable Flyer',
				'TYPE' => 'application/pdf',
				'URI' => ''
			),
			array(
				'NAME' => 'Facebook Event',
				'TYPE' => 'text/url',
				'URI' => 'http://facebook...'
			),
			array(
				'NAME' => 'jsonCalendar',
				'TYPE' => 'application/json',
				'URI' => ''
			),
			array(
				'NAME' => 'iCalendar',
				'TYPE' => 'text/calendar',
				'URI' => ''
			)
		);
		$organizers = array(
			'CN=Joe Blow:MAILTO:foo@example.com'
		);
		$timezones = array(
			array( 
				'TZID' => 'America/Los_Angeles',
				'STANDARD' => array(
					'TZNAME' => 'PST',
					'TZOFFSETFROM' => '-0700',
					'TZOFFSETTO' => '-0800',
					'DTSTART' => '19701101T0200000',
					'RRULE' => 'FREQ=YEARLY;BYMONTH=11;BYDAY=1SU'
				),
				'DAYLIGHT' => array(
					'TZNAME' => 'PDT',
					'TZOFFSETFROM' => '-0800',
					'TZOFFSETTO' => '-0700',
					'DTSTART' => '19700308T020000',
					'RRULE' => 'FREQ=YEARLY;BYMONTH=3;BYDAY=2SU'
				),
			)
		);
	}

}

