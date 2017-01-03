<?php
include("shared/global.cfg");
$request=$_SERVER['REQUEST_URI'];
$url="";
$urlhtml="";
if (ereg("article_id=",$request)) $uri = eregi_replace("3\?article_","?",$request);
if (ereg("id=",$request)) $url = eregi_replace("3\?","?",$request);

// Url Rewrites goes to .htaccess of here ;)
// if (ereg("/pdf/",$request)) $url="http://alternative-server-for-pdf.net$request";
// if ($request=="old/url/file.php") $url="/new/rul/";

if ($url) $urlhtml="<a href=\"$url\">http://$_SERVER[SERVER_NAME]$url</a><br>";
?><html>
<head>
<title>404 Missing Page - <?=$GLOBALS['site_nick']?></title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>

<body bgcolor="#FFFFFF" text="#000000">
<table width="468" border="0" cellspacing="0" cellpadding="3" align="center">
  <tr>
    <td valign="top"><a href="/"><img src="/images/imc_banner2.gif" 
width="468" height="60" border="0"></a></td>
  </tr>
  <tr>
    <td align="center"><font face="MS Sans Serif, Verdana, Arial, sans-serif" size="+2" color="#CC0000"><b>PAGE 
      NOT FOUND!<br>
	  <?php if ($urlhtml) { ?>
      <font size="-1" face="Verdana, Arial, Helvetica, sans-serif" color="#990000">
		But maybe this is the page you are looking for:<br>
<?php echo $urlhtml; ?></font><? } ?>
</b></font></td>
  </tr>
  <tr>
    <td align="center"><font face="Arial, Helvetica, sans-serif" size="-1">You 
      can go to our <a href="/">homepage</a>, browse our <a href="/news/">newswire</a>, 
      or look at our current news <a href="/features/">features</a>.</font></td>
  </tr>
</table>
</body>
</html>
