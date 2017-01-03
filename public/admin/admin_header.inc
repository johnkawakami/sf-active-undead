<?
//This file displays a common header for the amdin pages
//including the code that restricts these pages to those that have logged on
//If a user is no logged on this redirects them to the logon page

include_once(SF_SHARED_PATH . '/classes/newswire_class.inc');
include_once(SF_SHARED_PATH . '/classes/category_class.inc');

$sftr = new Translate();

$error_num = 0;

$session = array_merge($_SESSION, array(
	'is_editor' => null,
	'secure' => null,
	'user_id' => null,
));

if ($_SESSION['is_editor'] === true)
{
	// if it's one of the user editing screens, jump to the list.
	// not sure why this is special-cased.
	if($session['secure'] !== "yes" && preg_match("/(user_display_edit.php|user_update.php)/", $_SERVER['PHP_SELF']))
	{
		$new_loc = SF_ROOT_URL.'/admin/user/user_display_edit.php?user_id1='.$session['user_id'];
		header("Location: $new_loc");
		exit;
	}

	// otherwise, we display the admin header.
    include_once(SF_SHARED_PATH . '/classes/db_class.inc');

    $db_obj = new DB;
    $newswire_obj = new Newswire;
    $category_obj = new Category;

    if ($display) {
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
	// go to the logon screen
	$goto=urlencode($_SERVER['REQUEST_URI']);
	header("Location: /admin/authentication/authenticate_display_logon.php?goto=$goto");
	exit();
}

