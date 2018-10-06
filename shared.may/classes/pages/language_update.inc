<?php

// Class for language_update page

class language_update extends Page
{

    function language_update()
    {
        // In this kind of script, we can do everything in the constructor to avoid the whole mess
        $lang_obj=new \SFACTIVE\Language;
        $languagefields = array(
                                'id' => $_POST["id"],
                                'name' => $_POST["name"],
                                'language_code' => $_POST["language_code"],
				'order_num' => $_POST["order_num"],
				'display' => $_POST["display"],
				'build' => $_POST["build"],
                                );

        $error_num = $lang_obj->update($languagefields);
	$lang_obj->cache_language_select();
	$lang_obj->cache_builds();
	$lang_obj->cache_language_codes();
        $new_location = "language_display_edit.php?id=" . $_POST["id"];

        header("Location: $new_location");
        exit;
    }

    function execute() {
        // Execution method, does nothing
        return 1;
    }

}

?>
