<?php

// Class for language_reorder page
class language_reorder extends Page
{

    function language_reorder()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess

	$lang_obj = new language;
	$error_num = $lang_obj->reorder($_GET);
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
