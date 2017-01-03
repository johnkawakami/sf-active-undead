<?php

// Class for authenticate_display_logon page

class authenticate_display_logon extends Page {

    function authenticate_display_logon() {
        // Class constructor, does nothing
        return 1;
    }

    function execute() {
		$get = filter_input_array(INPUT_GET,array(
			'logon_failed' => FILTER_SANITIZE_STRING,
			'goto' => FILTER_VALIDATE_URL,
		));

        $tr = new Translate();

        if (strlen($get['logon_failed']) > 0) {
            $this->tkeys['local_error'] = "<p class=\"error\"><strong>" . $tr->trans('logon_failed') . "</strong></p>";
        } else {
            $this->tkeys['local_error'] = "";
        }

        $this->tkeys['sitenick'] = $GLOBALS['site_nick'];

        $this->tkeys['goto'] = $get['goto'];

        return 1;
    }

}

?>
