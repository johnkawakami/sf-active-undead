<?
//This file displays a common header for the amdin pages
//including the code that restricts these pages to those that have logged on
//If a user is no logged on this redirects them to the logon page

include_once(SF_SHARED_PATH . '/classes/newswire_class.inc');
include_once(SF_SHARED_PATH . '/classes/category_class.inc');

$sftr = new Translate();

$error_num = 0;

if ($_SESSION['is_editor'] === true)
{
	if($_SESSION['secure'] !== "yes" && !ereg("(user_display_edit.php|user_update.php)", $_SERVER['PHP_SELF']))
	{
		$new_loc = SF_ROOT_URL.'/admin/user/user_display_edit.php?user_id1='.$_SESSION['user_id'];
		header("Location: $new_loc");
		exit;
	}

    include_once(SF_SHARED_PATH . '/classes/db_class.inc');

    $db_obj = new DB;
    $newswire_obj = new Newswire;
    $category_obj = new Category;

    if ($display) {
        $Title = $subtitle;
        include(SF_INCLUDE_PATH . '/admin-header.inc'); 

?>

<script language="JavaScript" type="text/javascript">
<!--

function forceReload() {
    location.href = location.href + '?' + (new Date()).getTime();
    var lastTime = location.search.substring(1) - 0;
    if ((new Date()).getTime() - lastTime > 1000) {
        forceReload();
    }
}
//-->
</script>

<?php

    }
} else {
        $goto=urlencode($_SERVER['REQUEST_URI']);
	header("Location: /admin/authentication/authenticate_display_logon.php?goto=$goto");
	exit();
}

?>
