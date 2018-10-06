<?php // vim:et:ai:ts=4:sw=4
namespace SFACTIVE\Calendar;

/**
 * Manages the caching of calendar events and the one-week display.
 * Also caches a json copy of the event data, and provides
 * a utility function to retrieve the date information from 
 * that json file. That is to help construct a URL to display  
 * the calendar week in which the event happens.
 */
class CalendarCacheManager {
    private $path;

    function __construct($cachepath=null) {
        if (!$cachepath) { 
            $cachepath = SF_CACHE_PATH . '/calendar'; 
        }
        $this->path = $cachepath;
    }

    // methods for events

    public function cache_event($id) {
        $event = new Event();
        $event->load_by_id($id);

        // save json
        $json = EventJSON::to_json($event);
        file_put_contents($this->get_event_json_path($id), $json);

        // save html
        $event_display = new \SFACTIVE\Calendar\EventDisplay();
        $html = $event_display->renderEventHTML($id);
        file_put_contents($this->get_event_html_path($id), $html);
    }

    public function get_event_html($id) {
        if (!file_exists($path = $this->get_event_html_path($id))) {
            $this->cache_event($id);    
        }
        return file_get_contents($path);
    }

    public function get_event_json($id) {
        if (!file_exists($this->get_event_json_path($id))) {
            $this->cache_event($id);
        }
        return file_get_contents($this->get_event_json_path($id));
    }

    public function get_event_json_path($id) {
        return $this->get_event_path($id).'.json';
    }

    public function get_event_html_path($id) {
        return $this->get_event_path($id).'.html';
    }

    public function get_event_path($id) {
        $subfolder = intval($id) % 100;
        $path = $this->path.'/events/'.$subfolder.'/'.$id;
        $path = str_replace("//","/", $path);
        if (!file_exists(dirname($path))) {
            mkdir(dirname($path), 0777, true);
        }
        return $path;
    }

    // methods for weeks

    public function cache_week($date) {
        // $date->find_start_of_week();
        $event_display = new EventDisplay();
        $html_for_week = $event_display->renderWeekHTML($date, $filter);
        file_put_contents($this->get_week_html_path($date), $html_for_week);
    }
    public function get_week_html($date) {
        if (!file_exists($path = $this->get_week_html_path($date))) {
            $this->cache_week($date);
        }
        return file_get_contents($path);
    }

    public function get_week_html_path($date) {
        return $this->get_week_path($date).'.html';
    }

    public function get_week_path($date) {
        if ($date==null) { print_r(debug_backtrace()); }
        $weekname = $date->get_weekname();
        $path = $this->path.'/weeks/'.$weekname;
        $path = str_replace("//","/", $path);
        if (!file_exists(dirname($path))) {
            mkdir(dirname($path), 0777, true);
        }
        return $path;
    }

    /**
     * Returns info about the week into which the event falls.
     */
    public function get_event_week_info($id) {
        $json = json_decode($this->get_event_json($id), true);
        $info = array(
            'day' => $json['startDay'],
            'month' => $json['startMonth'],
            'year' => $json['startYear']
        );
        return $info;
    }
}

/**
 * Ad hoc way to turn an event into a JSON object, for writing to a file.
 */
class EventJSON {
    static function to_json($event) {
        return json_encode(
            array(
                "id" => $event->id,
                "title" => $event->title,
                "startTime" => $event->start_time,
                "endTime" => $event->end_time,
                "startDay" => $event->start_day,
                "endDay" => $event->end_day,
                "startMonth" => $event->start_month,
                "endMonth" => $event->end_month,
                "startYear" => $event->start_year,
                "endYear" => $event->end_year,
                "startDayOfWeek" => $event->start_day_of_week,
                "description" => $event->description,
                "eventTypeName" => $event->event_type_name,
                "eventTopicName" => $event->event_topic_name,
                "eventLocationName" => $event->event_location_name,
                "eventLocationDetails" => $event->event_location_details,
                "contactName" => $event->contact_name,
                "contactPhone" => $event->contact_phone,
                "contactEmail" => $event->contact_email,
                "file" => $event->file,
                "mimeType" => $event->mime_type,
                "languageId" => $event->language_id,
                "artMime" => $event->artmime
            )
        );
    }
}
