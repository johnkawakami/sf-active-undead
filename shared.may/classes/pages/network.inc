<?php

// Class for network page

class network extends Page
{

    function network()
    {
        // Class constructor, does nothing
        return 1;
    }

    function execute()
    {
        // we are only assigning variables here

        $tr = new Translate();
        $this->tkeys['local_cities_list'] = '';

        $cities = file (SF_SHARED_PATH . '/cities.inc');
        while (list ($line_num, $line) = each ($cities)) 
        {
            $this->tkeys['local_cities_list'] .= $line . "<br />\n";
        }

        $this->tkeys['local_site_crumb'] = $GLOBALS['site_crumb'];
        $this->tkeys['local_site_nick']  = $GLOBALS['site_nick'];
        $this->tkeys['local_refurl']     = SF_ROOT_URL . $_SERVER['PHP_SELF'] . "?" . $_SERVER['QUERY_STRING'];
        $this->tkeys['local_updated']    = $tr->trans('last_updated') . " " . date("r",filemtime(SF_SHARED_PATH . '/cities.inc'));
    }

} // end class
?>