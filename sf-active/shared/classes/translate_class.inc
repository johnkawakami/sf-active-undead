<?php

// This class sets up translation and internationalization

class Translate {

    function Translate ()
    {
        // Constructor creates common language table
        $local_filename = SF_INSTALL_PATH . '/local/languages/' . $GLOBALS['lang'] . '/common.dict';
        $global_filename = SF_SHARED_PATH . '/dictionary/' . $GLOBALS['lang'] . '/common.dict';

        if (file_exists($local_filename))
        {
            include($local_filename);
        } elseif (file_exists($global_filename))
        {
            include($global_filename);
        } else
        {
            return 0;
        }

        return 1;
    }

    function trans ($key)
    {
		if (!isset($GLOBALS['dict'][$key])) return;
        if (strlen($GLOBALS['dict'][$key]) > 0) return $GLOBALS['dict'][$key];
    }

    function create_translate_table ($keyword)
    {
        if (strlen($keyword) > 0)
        {
            // this should be changed so that creating a table without a keyword is ok

            $splitname = split("_", $keyword);

            $filename  = $GLOBALS['lang'] . "/" . $keyword . ".dict";
            $filename2 = $GLOBALS['lang'] . "/" . $splitname[0] . ".dict";

            $local_filename  = SF_INSTALL_PATH . '/local/languages/' . $filename;
            $local_filename2 = SF_INSTALL_PATH . '/local/languages/' . $filename2;

            $global_filename  = SF_SHARED_PATH . '/dictionary/' . $filename;
            $global_filename2 = SF_SHARED_PATH . '/dictionary/' . $filename2;

            // adding check for local dictionary giving precedence over global dictionary

            if (file_exists($local_filename))
            {
                include($local_filename);
            } elseif (file_exists($global_filename))
            {
                include($global_filename);
            } else
            {
                $local = '';
            }

            if (is_array($local))
            {
                $GLOBALS['dict'] = array_merge($GLOBALS['dict'], $local);
            }

            if (file_exists($local_filename2))
            {
                include($local_filename2);
            } elseif (file_exists($global_filename2))
            {
                include($global_filename2);
            } else
            {
                $local = '';
            }

            if (is_array($local))
            {
                $GLOBALS['dict'] = array_merge($GLOBALS['dict'], $local);
            }

        } else
        {
            return 0;
        }
        return 1;
    }

}

?>
