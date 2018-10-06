<?php

// Class for language_hide page

class language_hide extends Page
{

    function language_hide()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess
        $lang_obj = new language;
	if(isset($_GET['hide']))
	{
	    $languagefields =   array(
                          id =>   $_GET['hide'],
                              );
	    $error_num = $lang_obj->hide($languagefields);
	}
	if(isset($_GET['show']))
	{
            $languagefields =   array(
                          id =>    $_GET['show'],
                                );
        $error_num = $lang_obj->show($languagefields);
	}
	$lang_obj->cache_language_select();
	$lang_obj->cache_builds();
	$lang_obj->cache_language_codes();
        header("Location: /admin/language/language_display_list.php");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
