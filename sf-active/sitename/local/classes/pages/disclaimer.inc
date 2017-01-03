<?php
// Class for Disclaimer

class disclaimer extends LocalPage {

    function disclaimer () {
        // Class constructor, does nothing
        return 1;
    }

    function execute () {

		// Translate all TPL_ in the Template
		$tr = new Translate();
		$tr->create_translate_table('disclaimer');

		$this->tkeys['SITE_NAME'] = $GLOBALS['site_name'];
		$this->tkeys['SITE_CRUMB'] = $GLOBALS['site_crumb'];
		$this->tkeys['LANG_DISCLAIMER'] = $tr->trans('disclaimer');
		$this->tkeys['LANG_PRIVACY'] = $tr->trans('privacy');

	}
	
}

?>





