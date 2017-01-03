<?php

require_once('../../sf-active/shared/global.cfg');
require_once(SF_SHARED_PATH.'/classes/category_class.inc');
require_once(SF_SHARED_PATH.'/classes/db_class.inc');

//
// Automatically Regenerate Missing Files Based on URI
//
// To install this script, add the following to your .htaccess
// or in the right directory in httpd.conf:
//
// ErrorDocument 404 /errors/404error.php
//
// -johnk (of LA Indy)
//

$request_uri = $_SERVER["REQUEST_URI"];
$referrer = isset($_SERVER["HTTP_REFERER"]) ? $_SERVER['HTTP_REFERER'] : null;
$id = -1;

// If it's a graphics URL, send them to the failure image
if (preg_match('/(gif|jpg|png)$/i', $request_uri))
{
	// if it's not local, they get the error
	if (! preg_match('#^http://la.indymedia.org#',$referrer))
	{
		header('HTTP/1.0 404 Not Found', 404);
		exit;
		header('Location: http://la.indymedia.org/uploads/failed.gif');
		exit;
	}
	else
	{
		// if it's local, find the image in the filesystem
		// and redirect the browser to that image
		// -jk
		include_once("shared/global.cfg");
		$matches = array();
		preg_match('/(\/[^\/]+\.)(gif|jpg|png)$/i', $request_uri, $matches);
		$filename = $matches[1].$matches[2];
		$db = new DB();
		$q = "SELECT linked_file FROM webcast WHERE linked_file LIKE '%$filename'";
		$res = $db->query($q);
		$filepath = $res[0]['linked_file'];
		$parts = split('/', $filepath);
		$tail = join('/',array_slice($parts,-3));
		$url = 'http://la.indymedia.org/uploads/'.$tail;
		header('Location: '.$url);
		exit;
	}
}

//
// If it looks like a news URL, then try to regenerate the
// article and then forward the user to that page.
//
if (preg_match( '#^/news/(\d+?)/(\d+?)/(\d+?)(\.json|_comment\.php|\.php|\.html)#',
                $request_uri, $matches )) 
{

    include_once("shared/global.cfg");
	
    $year  = $matches[1];
	$month = $matches[2];
    $id    = $matches[3];
	$comm  = $matches[4];

	if (!filter_var($id,FILTER_VALIDATE_INT)) {
		echo "invalid id";
		goto defaultpage;
	}

	$db = new DB();
	$res = $db->query("SELECT COUNT(*) AS c FROM webcast WHERE id=$id");

	if ($res[0]['c']==0) {
		#http_response_code(410);
		header('HTTP/1.0 410 Gone', 410);
		echo "nonexistent id $id";
		exit;
	}

	$article = new Article($id);
	if ($article->article['display'] != 'f') 
	{
		$article->render_everything();
		$article->cache_to_disk();

		// New uri is necessary because the original may have had the wrong date.
		$year = $article->article['created_year'];
		$month = $article->article['created_month'];
		$new_uri = "/news/$year/$month/$id$comm";

		header("Location: $new_uri");
		# if you get redirect errors, comment out the above.  you will see 
		# the php error messages
		#echo "<a href=$new_uri>$new_uri</a>";

		exit;
	}	
}
defaultpage:
// default behavior
?>
<!doctype html>
<html>
 <head>
  <style type="text/css">
	body { font-family: Arial; }
	.box { background-color: #eee; border: 1px solid silver; }
	.box span { line-height: 60px; margin-left: 20px; vertical-align: middle; }
	.box2 { padding: 20px; background-color: #fff; border: 1px solid silver; border-top: 0; }
	img { vertical-align: bottom; }
  </style>
 </head>
 <body>
<div class='box'>
<a href="/">
<img src='http://la.indymedia.org/images/ilogo.gif' /></a>
<span><a href="http://la.indymedia.org">LA Indymedia</a></span>
</div>
<div class='box2'>
<h1>Error 404: document not found</h1>

<p><?=$request_uri?> was not found on this server.</p>

<p>It's possible that this article has a status of "hidden".  If you think that's the problem, 
try this url: <a href="/news/hidden.php?id=<?=$id?>">/news/hidden.php?id=<?=$id?></a></p>
</div>

<? include("footer.html") ?>

</body>
</html>
