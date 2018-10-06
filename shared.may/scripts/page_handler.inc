<?php

// This page will stay in scripts until site-wide user handling is implemented
// It handles theme and language cookie settings

$language = $GLOBALS['lang'];

if ($_COOKIE['language'])
{
    switch ($_COOKIE['language'])
    {
	case "ca":
	    $language="ca";
	    break;
    case "en":
            $language="en";
            break;
    case "es":
            $language="es";
            break;
    case "fr":
            $languag="fr";
            break;
	case "hr":
			$language="hr";
			break;
    case "it":
            $language="it";
            break;
	case "nl":
	    $language="nl";
	    break;
	case "pt":
	    $language="pt";
	    break;
	case "sk":
	    $language="sk";
	    break;
	case "sr":
	    $language="sr";
	    break;
	case "tr":
	    $language="tr";
	    break;
        default:
            $language = $GLOBALS['lang']; 
    }
}

if ($_GET['l'])
{
    switch ($_GET['l'])
    {
	case "ca":
	    $language = "ca";
	    setcookie("language", "ca", 0, "/");
	    break;

        case "en":
            $language = "en";
            setcookie("language", "en", 0, "/");
            break;

        case "es":
            $language = "es";
            setcookie("language", "es", 0, "/");
            break;

        case "fr":
            $language = "fr";
            setcookie("language", "fr", 0, "/");
            break;

		case "hr":
			$language = "hr" ;
			setcookie("language", "hr", 0, "/");
			break;

        case "it":
            $language = "it";
            setcookie("language", "it", 0, "/");
            break;

	case "nl":
	    $language = "nl";
	    setcookie("language", "nl", 0, "/");
	    break;

        case "pt":
            $language = "pt";
            setcookie("language", "pt", 0, "/");
            break;

        case "sk":
            $language = "sk";
            setcookie("language", "sk", 0, "/");
            break;

        case "sr":
            $language = "sr";
            setcookie("language", "sr", 0, "/");
            break;

	case "tr":
	    $language = "tr" ;
	    setcookie("language", "tr", 0, "/");
	    break;

        default:
            $language = $GLOBALS['lang'];
    }
}

$GLOBALS['lang'] = $language;

function get_theme() {
	if ($_COOKIE['sf_theme']) {
		return $_COOKIE['sf_theme'];
	} else if ($GLOBALS['sf_theme']) {
		return $GLOBALS['sf_theme'];
	}
	return 'theme0.css';
}

$GLOBALS['sf_theme'] = get_theme(); 


// Here is where the admin session code is being moved in order to accommodate the new page include scheme

if (preg_match("/admin/", $_SERVER['PHP_SELF']))
{
    ini_set('session.save_path', SF_SESSIONS_PATH);
    session_start();
}

// This is here as a weird fix for a problem with creating the url's, it should be removed eventually
if (preg_match("/\?/",$_SERVER['REQUEST_URI']))
{
    $punct="&amp;";
} else {
    $punct="?";
}

$thematic = $_SERVER['REQUEST_URI'] . $punct;
