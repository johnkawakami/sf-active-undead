<?php

// Class for the Calender Add and Update Form

class event_display_add_confirm extends Page {

    function event_display_add_confirm () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {
	
		include SF_CALENDAR_PATH."/common_include.inc";

		// Translate all TPL_ in the Template
		$tr = new Translate();
		$tr->create_translate_table('event_display_add_confirm');
		$this->translation($tr);

		$this->tkeys['CAL_CONFIRM'] = $_GET['confirmation_number'];
		return 1;		
	}

	function translation ($tr) {
	
		$this->tkeys['CAL_LANG_CONFIRM_ADD_PAGE'] = $tr->trans('cal_confirm_add_page');
		$this->tkeys['CAL_LANG_YOUR_CONFIRM'] = $tr->trans('cal_your_confirm');
		$this->tkeys['CAL_LANG_NEED_CONFIRM'] = $tr->trans('cal_need_confirm');
		$this->tkeys['CAL_LANG_RETURN_TO_CALENDAR'] = $tr->trans('cal_return_to_calendar');
	
	}
	
}
