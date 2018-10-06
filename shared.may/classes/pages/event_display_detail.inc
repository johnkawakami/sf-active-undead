<?php // vim:et:ts=4:sw=4:st=4

include SF_CALENDAR_PATH."/common_include.inc";     
include_once SF_CLASS_PATH."/firewall_class.inc";

// Class for the Calender Detail Page...
class event_display_detail extends Page {

    function event_display_detail ($settings) {
        $this->settings = $settings;
        $this->get = filter_input_array( INPUT_GET, [
            'event_id' => FILTER_SANITIZE_NUMBER_INT
        ] );
        $this->get = coerce_array_values_to_int($this->get, ['event_id']);

        // detect some SQL injection hacks I have found - johnk
        if (preg_match('/UNION/', $_GET['event_id'])) {
            Firewall::add($_SERVER['REMOTE_ADDR']);
        }
        return 1;
    }

    function execute () {
        //This displays a cached file for an event        

        // Translate all TPL_ in the Template
        $tr = new Translate();
        $tr->create_translate_table('event_display_detail');        
        $this->translation($tr);

        $id = intval($this->get['event_id']);

        $this->tkeys['CAL_EVENT_ID'] = $id;
        
        try {
            $ccm = $GLOBALS['calendar_cache_manager7'];
            $this->tkeys['CAL_CACH_FILE'] = $ccm->get_event_html($id);
            $date_info = $ccm->get_event_week_info($id);
            $this->tkeys['CAL_DAY'] = $date_info['day'];
            $this->tkeys['CAL_MONTH'] = $date_info['month'];
            $this->tkeys['CAL_YEAR'] = $date_info['year'];
        } catch(CalendarEventNotFoundException $e) {
            $this->tkeys['CAL_CACH_FILE'] = $e->getMessage();
            $this->settings['noindex'] = true;
            $this->settings['http_status'] = 410;
        } catch(Exception $e) {
            $this->tkeys['CAL_CACH_FILE'] = $e->getMessage();
            debug_print_backtrace();
        }
    }

    function translation ($tr) {
        $this->tkeys['LANG_SITE_CRUMB'] = $GLOBALS['site_crumb'];

        $this->tkeys['CAL_LANG_CALENDAR'] = $tr->trans('cal_calendar');
        $this->tkeys['CAL_LANG_EVENT_DETAIL'] = $tr->trans('cal_event_detail');        
        $this->tkeys['CAL_LANG_RETURN_TO_CAL'] = $tr->trans('cal_return_to_cal');
        $this->tkeys['CAL_LANG_UPDATE'] = $tr->trans('cal_update');
        $this->tkeys['CAL_LANG_DELETE'] = $tr->trans('cal_delete');
    }
    
}

?>
