<?php // vim:et:ai:ts=4:sw=4

include('shared/vendor/autoload.php');
include_once('shared/global.cfg');
include_once('shared/classes/category_class.inc');

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
$referrer = $_SERVER["HTTP_REFERER"];
$another_uri = $_GET['uri'];

if (preg_match('#/news/\d+?/\d+?/\d+?(|_comment\.php|_comment\.html|\.php|\.html|\.json)$#', $another_uri)) {
    $request_uri = $another_uri;
}

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
		$db = new SFACTIVE\DB();
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
if (preg_match( '#^/news/(\d+?)/(\d+?)/(\d+?)(|\.json|_comment\.php|\.php|_comment\.html|\.html)$#',
                $request_uri, $matches )) 
{

	include_once("shared/global.cfg");
	
	$year  = $matches[1];
	$month = $matches[2];
	$id    = $matches[3];
	$comm  = $matches[4];

	if (!filter_var($id,FILTER_VALIDATE_INT)) {
		// bad id - we don't normally get here
		showpage("Error 400: Bad Request", null, $request_uri);
	}

	$db = new SFACTIVE\DB();

	// if this ID has a redirect, then we 301 them to that address.
	$res = $db->query("SELECT url FROM redirect WHERE id=$id");
	if ($res[0]['url']) {
		header('Location: '.$res[0]['url'], true, 301);
		header('HTTP/1.0 301', 301);
		exit();
	}

	$res = $db->query("SELECT COUNT(*) AS c FROM webcast WHERE id=$id");

	if ($res[0]['c']==0) {
		#http_response_code(410);
		header('HTTP/1.0 410 Gone', 410);
		showpage("Error 410: Gone", null, $request_uri);
	}

	// if the URL is to a comment, then we render the parent.
	$res = $db->query("SELECT parent_id FROM webcast WHERE id=$id");
	if ($res[0]['parent_id']!=0) {
		$id = $res[0]['parent_id'];
	}

	$article = new SFACTIVE\Article($id);
	if ($article->article['display'] != 'f') 
	{
		$article->render_everything();
		$article->cache_to_disk();

		// New uri is necessary because the original may have had the wrong date.
		$year = $article->article['created_year'];
		$month = $article->article['created_month'];
		$new_uri = "/news/$year/$month/$id$comm";
		if ($comm=='') { $new_uri .= '.php'; }

		http_response_code(301);
		header("Location: $new_uri");
        echo "Location: $new_uri";
		exit;
	}	
}

if (preg_match('#uploads/.*/.*/*.*$#', $request_uri)) {
    header('HTTP/ 410 Gone', 410);
    showpage( "Error 410: Gone Daddy Gone, File is Gone", null, $request_uri );
}
if (preg_match('#news/.*/.*/(.*)$#', $request_uri, $matches)) {
    if (!preg_match('/^[0-9]+\.[a-z]+$/', $matches[1])) {
        header('HTTP/ 410 Gone', 410);
        showpage( "Error 410: Gone, but was it ever there? There may be a relative link on a page.", null, $request_uri );
    }
}
http_response_code(410);
showpage( "Error 410: Page is gone.", $id, $request_uri );


function showpage( $title, $id, $request_uri ) {
	?>
	<?php include("header.html"); ?>
	<div class='box2'>
	<h1><?=$title?></h1>

	<p><?=$request_uri?> was not found on this server.</p>

	<?php if ($id) { ?>
		<p>It's possible that this article has a status of "hidden".  If you think that's the problem, 
		try this url: <a href="/news/hidden.php?id=<?=$id?>">/news/hidden.php?id=<?=$id?></a></p>
	<?php } ?>
	</div>
	<?php include("footer.html"); ?>
	<?php
	exit();
}

