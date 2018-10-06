<?php

// a handler for the theme.php page

// theme activation code has been removed from page_handler.inc,
// so ?theme=x doesn't work on any url except theme.php.

if (isset($_GET['theme'])) {
	$theme = intval($_GET['theme']);
	if (0 <= $theme  and $theme <= 9) {
		$themename = 'theme'.$theme.'.css';
		setcookie('sf_theme', $themename, time() + 500000, '/');
		header("Location: /theme.php");
		exit();
	}
}

