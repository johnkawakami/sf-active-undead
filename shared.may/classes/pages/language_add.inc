<?php

// Class for language_add page

class language_add extends Page
{

    function language_add()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess
        $lang_obj = new language;
        $languagefields =   array(
		name =>             $_POST['name'],
                language_code =>    $_POST['language_code'],
		order_num =>	    $_POST['order_num'],
		display =>	    $_POST['display'],
		build =>	    $_POST['build'],
                );

        $error_num = $lang_obj->add($languagefields);
	if($error_num < 0) print($error_num);
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
