<?php

// this kind of code and other stuff is what should be moved out per occam's idea

$headerdate = strtolower(date("l, F j, Y"));
$url_string = "";

foreach ($_GET as $key => $value)
{
    if ($key != "theme")
    {
        $url_string .= "&" . $key . "=" . $value;
    }
}

include(SF_INSTALL_PATH . '/local/include/wapdetect.inc');
if ($GLOBALS['isMobile']) {
	if ($GLOBALS['page_id']>0) {
		header("Location: /js/getarticle.php?id=".$GLOBALS['page_id']);
		exit();
	}
} else {

	if ($GLOBALS['http_status']!=200) {
		http_response_code($GLOBALS['http_status']);
	}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html><head><!-- content-header.inc -->
<title><?php if ($GLOBALS['page_title']) { echo $GLOBALS['page_title'] . " : ".$GLOBALS['site_nick']; } else { echo $GLOBALS['site_name']; } ?></title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<?php if (($GLOBALS['page_display']=='f')or($_GET['display']=='f')) { ?><meta name="ROBOTS" content="NOINDEX, NOFOLLOW, NOARCHIVE" />
<?php } ?>
<?php if ($GLOBALS['noindex'] or in_array(basename($_SERVER['SCRIPT_FILENAME']), array('theme.php', 'hidden.php', 'comment.php', 'publish.php'))) {
	echo '<meta name="ROBOTS" content="NOINDEX" />';
} 
if ($GLOBALS['canonical_url']) { echo '<link rel="canonical" href="'.$GLOBALS['canonical_url'].'" />'; }
?>
<?php
	if ($GLOBALS['page_title']) {
		?><meta name="og:title" content="<?php echo str_replace('"','\\"',$GLOBALS['page_title'])?>" /><?php
	}

	if ($GLOBALS['page_description'] != '') {
		?> 
        <meta name="og:description" content="<?php echo str_replace('"','\\"',strip_tags($GLOBALS['page_description']))?>" />
		<META NAME="Description" CONTENT="<?php echo str_replace('"','\\"',strip_tags($GLOBALS['page_description']))?>" />
        <?php
	} else {
		?>
		<META NAME="Description" CONTENT="The Los Angeles Independent Media Center publishes progressive activist news in Los Angeles and including parts of Ventura, Santa Barbara, Riverside, and Orange counties.  Leftist perspectives, including social democrat, anarchist, communist, socialist, and revolutionary voices. Articles are written by the readers."><!-- <?=$GLOBALS['page_description'];?> -->
		<?php
	}
?>
<link rel="stylesheet" type="text/css" href="/themes/<?php echo $GLOBALS['sf_theme']; ?>" />
<link rel="alternate" type="application/rss+xml" title="<?php if ($GLOBALS['page_title']) { echo $GLOBALS['page_title'] . " : ".$GLOBALS['site_nick']; } else { echo $GLOBALS['site_name']; } ?>"
href="http://la.indymedia.org/newswire.rss">
<script type="text/javascript" src="/qc/qc.js"></script>
</head>
<body>
<?php if ($GLOBALS['isMobile']) { ?>
		<div style="padding: 20px; width: 100%; background-color: #eee; color: black; text-align: center; font-size:40pt;"><a href="/js/getarticle.php?id=<?php echo $GLOBALS['page_id'] ?>">Try our new mobile site, in development</a></div>
<?php } ?>
<!-- <table width="100%" border="0" cellspacing="0" cellpadding="0"
class="bgult"><tr><td width="100%"> --><table width="100%" border="0" cellspacing="0"
cellpadding="0"><tr><td class="bgpenult">

<div class="mast">

<a name="top" href="/"><img align="absmiddle" alt="imc" src="/images/ilogo.gif" width="60" height="60" border="0" style="float:left; margin-right: 20px;" /></a>

<?php switch( $GLOBALS['sf_theme'] ) {
	case 'theme1.css': $image='/images/sfindyblack.gif'; $width='148'; break;
	case 'theme4.css': $image='/images/sfindyblack.gif'; $width='148'; break;
	case 'theme6.css': $image='/images/sfindyblack.gif'; $width='246'; break;
	case 'theme7.css': $image='/im/bump.gif'; $width='1'; break;
	case 'theme8.css': $image='/im/bump.gif'; $width='1'; break;
	case 'theme9.css': $image='/images/sfindyblack.gif'; $width='148'; break;
	default: $image='/im/bump.gif'; $width='148'; break;
} 
?>

<?php switch( $GLOBALS['sf_theme'] ) {
	case 'theme1.css': $image = 'logoblack.jpg'; $width = '418'; break;
	case 'theme4.css': $image = 'logoblack.jpg'; $width = '418'; break;
	case 'theme7.css': $image = 'imc_banner.gif'; $width = '468'; break;
	case 'theme8.css': $image = 'imc_banner2.gif'; $width = '468'; break;
	case 'theme9.css': $image = 'blood.gif'; $width = '418'; break;
	default: $image = 'logo.jpg'; $width = '418'; break;
} ?><img align="right" alt="indymedia" src="/images/<?=$image?>" width="<?=$width?>" height="60" border="0" style="float: right;" />
<h1 style="display:block; text-align:left; padding:15px 10px 10px 10px; font-size:15px;color:#aaa; height:20px;">Los Angeles Indymedia : Activist News</h1>

</div>

</td></tr>

<tr><td align="center" class="bgnavbar">

<span style="width:70px; float: right; margin-top: 1px;"><a href="/theme.php?theme=default"><img alt="white theme" align="middle" src="/images/box-white.gif" width="13" height="13" border="0" hspace="2" /></a><a href="/theme.php?theme=1"><img align="middle" src="/images/box-black.gif" alt="black theme" width="13" height="13" border="0" hspace="2" /></a><a href="/theme.php?theme=2"><img src="/images/box-red.gif" alt="red theme" width="13" height="13" align="middle" border="0" hspace="2" /></a><a href="/theme.php"><img src="/images/box-help.gif" alt="theme help" align="middle" width="13" height="13" border="0" hspace="2" /></a></span>

<table width="800" border="0" cellspacing="0" cellpadding="0"><tr>
<th width="15%"><a class="navbar" href="/process/about.php"><?php echo $sftr->trans('about_us'); ?></a></th>
<th width="15%"><a class="navbar" href="/process/contact.php"><?php echo $sftr->trans('contact_us'); ?></a></th>
<th width="15%"><a class="navbar" href="/calendar/"><?php echo $sftr->trans('calendar'); ?></a></th>
<th width="15%"><a class="navbar" href="/publish.php"><?php echo $sftr->trans('publish'); ?></a></th>
<th width="15%"><a class="navbar" href="/newswire.rss">RSS</a></th>
</tr></table>

</td></tr>

<?php if ($left=="noleft") { echo "</table></td></tr></table>"; } else { include(SF_INSTALL_PATH . '/local/include/left_center.inc'); } ?>
<!-- end content-header.inc --><?php } // isglobal ?>
