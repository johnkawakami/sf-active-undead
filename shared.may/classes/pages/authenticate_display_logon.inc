<?php

// Class for authenticate_display_logon page

class authenticate_display_logon extends Page {

    function authenticate_display_logon() {
        // Class constructor, does nothing
        return 1;
    }

    function execute() {

        $tr = new Translate();

        if (strlen($_GET['logon_failed']) > 0) {
            $this->tkeys['local_error'] = "<p class=\"error\"><strong>" . $tr->trans('logon_failed') . "</strong></p>";
        } else {
            $this->tkeys['local_error'] = "";
        }

        $this->tkeys['sitenick'] = $GLOBALS['site_nick'];

        $this->tkeys['goto'] = $_GET['goto'];

        return 1;
    }

}

?>
