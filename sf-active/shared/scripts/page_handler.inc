<?php

// This page will stay in scripts until site-wide user handling is implemented
// It handles theme and language cookie settings

$language = $GLOBALS['lang'];

if (isset($_COOKIE['language']))
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

if (isset($_GET['l']))
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

// if no default theme is set in local config set a global default theme
$GLOBALS['sf_theme'] = (isset($GLOBALS['sf_theme']) && $GLOBALS['sf_theme']) ? $GLOBALS['sf_theme'] : 'theme0.css';

// include_once('/usr/local/sf-active/sitename/public/mirror/cache/cache.php');
// now set the themes to users cookie
$GLOBALS['sf_theme'] = (isset($_COOKIE['sf_theme']) && $_COOKIE['sf_theme']) ? $_COOKIE['sf_theme'] : $GLOBALS['sf_theme']; 

// get a theme change from CGI and set the cookie
if (isset($_GET['theme']))
{
	$e = time() + 500000;
    switch($_GET['theme'])
    {
        case "default": 
            setcookie("sf_theme","theme0.css", $e,"/");
            $GLOBALS['sf_theme'] = "theme0.css";
            break;
        case "1":
            setcookie("sf_theme","theme1.css", $e,"/");
            $GLOBALS['sf_theme'] = "theme1.css";
            break;
        case "2":
            setcookie("sf_theme","theme2.css", $e,"/");
            $GLOBALS['sf_theme'] = "theme2.css";
            break;
        case "3":
            setcookie("sf_theme","theme3.css", $e,"/");
            $GLOBALS['sf_theme'] = "theme3.css";
            break;
        case "4":
            setcookie("sf_theme","theme4.css", $e,"/");
            $GLOBALS['sf_theme'] = "theme4.css";
            break;
        case "5":
            setcookie("sf_theme","theme5.css", $e,"/");
            $GLOBALS['sf_theme'] = "theme5.css";
            break;
        case "6":
            setcookie("sf_theme","theme6.css", $e,"/");
            $GLOBALS['sf_theme'] = "theme6.css";
            break;
        case "7":
            setcookie("sf_theme","theme7.css", $e,"/");
            $GLOBALS['sf_theme'] = "theme7.css";
            break;
        case "8":
            setcookie("sf_theme","theme8.css", $e,"/");
            $GLOBALS['sf_theme'] = "theme8.css";
            break;
        case "9":
            setcookie("sf_theme","theme9.css", $e,"/");
            $GLOBALS['sf_theme'] = "theme9.css";
            break;
        default:
            setcookie("sf_theme","theme0.css", $e,"/");
            $GLOBALS['sf_theme'] = "theme0.css";
    }
}

// Here is where the admin session code is being moved in order to accommodate the new page include scheme

if (eregi("admin", $_SERVER['PHP_SELF']))
{
    ini_set('session.save_path', SF_SESSIONS_PATH);
    session_start();
}

// This is here as a weird fix for a problem with creating the url's, it should be removed eventually
if (ereg("\?",$_SERVER['REQUEST_URI']))
{
    $punct="&amp;";
} else {
    $punct="?";
}

$thematic = $_SERVER['REQUEST_URI'] . $punct;
