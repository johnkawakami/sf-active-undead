<?php

// this kind of code and other stuff is what should be moved out per occam's idea

$headerdate = strtolower(date("l, F j, Y"));
$url_string = "";

foreach ($_GET as $key => $value)
{
    if ($key != "theme")
    {
        $url_string .= "&amp;" . $key . "=" . $value;
    }
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html><head>
<title><?php if ($GLOBALS['page_title']) { echo $GLOBALS['page_title'] . " : ".$GLOBALS['site_nick']; } else { echo $GLOBALS['site_name']; } ?></title>
<?if($GLOBALS['lang'] == "tr")
{?>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-9" />
<? ;}
elseif($GLOBALS['lang'] == "hr")
{?>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-2" />
<? ;}
else
{?>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<? ; } ?>
<?php if (($GLOBALS['page_display']=='f')or($_GET['display']=='f')) { ?><meta name="ROBOTS" content="NOINDEX, NOFOLLOW, NOARCHIVE" />
<?php } ?>
<link rel="stylesheet" type="text/css" href="/themes/<?php echo $GLOBALS['sf_theme']; ?>" />
</head>

<body><table width="100%" border="0" cellspacing="0" cellpadding="0"
class="bgult"><tr><td><table width="100%" border="0" cellspacing="1"
cellpadding="0"><tr><td class="bgpenult"><table class="mast" width="100%"
border="0" cellspacing="0" cellpadding="0"><tr class="mast"><td
class="mast" width="60"><a name="top" href="/"><img align="middle" alt="imc"
src="/images/ilogo.gif" width="60" height="60" border="0" /></a></td><td
class="mast"><a href="/"><img align="middle" src="<? if (($GLOBALS['sf_theme']=="theme1.css") or ($GLOBALS['sf_theme']=="theme9.css") or ($GLOBALS['sf_theme']=="theme4.css")) { echo "/images/sfindyblack.gif"; } 
else if ($GLOBALS['sf_theme']=="theme6.css") { echo "/images/sfbayarea.gif"; }
else if ($GLOBALS['sf_theme']=="theme8.css") { echo "/im/bump.gif"; }
else if ($GLOBALS['sf_theme']=="theme7.css") { echo "/im/bump.gif"; }
else { echo "/images/sfindy.gif"; } ?>" alt="sf.indymedia.org" border="0" width="<? if ($GLOBALS['sf_theme']=="theme6.css") { echo "245"; }
else if ($GLOBALS['sf_theme']=="theme8.css" or $GLOBALS['sf_theme']=="theme7.css") { echo "1"; }
else { echo "148"; } ?>" height="26" /></a></td><td width="418" class="mast" align="right"><a
href="/"><img align="middle" alt="san francisco bay area indymedia"
src="/images/<? if (($GLOBALS['sf_theme']=="theme1.css") or ($GLOBALS['sf_theme']=="theme4.css")) { echo "imcsfobb16.gif"; }
else if ($GLOBALS['sf_theme']=="theme9.css") { echo "blood.gif"; } 
else if ($GLOBALS['sf_theme']=="theme8.css") { echo "imc_banner2.gif"; } 
else if ($GLOBALS['sf_theme']=="theme7.css") { echo "imc_banner.gif"; } 
else { echo "logo.jpg"; } ?>" width="<?php if ($GLOBALS['sf_theme']=="theme8.css" or $GLOBALS['sf_theme']=="theme7.css") { echo "468"; }
else { echo "418"; } ?>" height="60" border="0" /></a></td></tr></table></td></tr>

<tr><td align="center" class="bgnavbar"><table width="100%" border="0" 
cellspacing="0" cellpadding="0"><tr>

<th width="20%"><a class="navbar" href="/process/about.php"><?php echo $sftr->trans('about_us'); ?></a></th>

<th width="20%"><a class="navbar" href="/process/contact.php"><?php echo $sftr->trans('contact_us'); ?></a></th>

<th width="20%"><a class="navbar" href="/mailinglist.php"><?php echo $sftr->trans('subscribe_me'); ?></a></th>

<th width="20%"><a class="navbar" href="/calendar/"><?php echo $sftr->trans('calendar'); ?></a></th>

<th width="20%"><a class="navbar" href="/publish.php"><?php echo $sftr->trans('publish'); ?></a></th>

<td width="52" align="right"><table cellpadding="0" cellspacing="0" 
border="0"><tr><td><a href="?theme=default<?php echo $url_string ?>"><img alt="white theme" 
align="middle" src="/images/box-white.gif" width="13" 
height="13" border="0" hspace="2" /></a></td><td><a href="?theme=1<?php echo $url_string ?>"><img 
align="middle" src="/images/box-black.gif" alt="black theme" width="13" height="13" border="0" hspace="2" /></a></td><td><a 
href="?theme=2<?php echo $url_string ?>"><img src="/images/box-red.gif" alt="red theme" width="13" height="13" 
align="middle" border="0" hspace="2" /></a></td><td><a 
href="/theme.php"><img src="/images/box-help.gif" alt="theme help" 
align="middle" width="13" height="13" border="0" hspace="2" /></a></td></tr></table></td></tr></table></td></tr>

<? if ($left=="noleft") { echo "</table></td></tr></table>"; } else { include(SF_INSTALL_PATH . '/local/include/left_center.inc'); } ?>
