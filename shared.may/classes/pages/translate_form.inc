<?php

// Class for translate_form page

class translate_form extends Page
{

    function translate_form()
    {
        // Class constructor, does nothing
        return 1;
    }

    function execute()
    {
        // we are only assigning variables here

        $tr = new Translate();

        $this->tkeys['local_site_crumb'] = $GLOBALS['site_crumb'];
        $this->tkeys['local_site_nick']  = $GLOBALS['site_nick'];
        $this->tkeys['local_refurl']     = SF_ROOT_URL . $_SERVER['PHP_SELF'] . "?" . $_SERVER['QUERY_STRING'];
    }

} // end class
?>