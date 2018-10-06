<?php // vim:et:ai:sw=4:ts=4
include "shared/vendor/autoload.php";

use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\HttpFoundation\Cookie;  

include_once("shared/global.cfg");

function too_many_reports( $ip, $interval_hours, $max_reports_per_interval )
{
	$db = new SFACTIVE\DB();
	$a = $db->queryFetchOne("SELECT COUNT(*) as c FROM qc 
                        WHERE ip=:ip 
                        AND	DATE_SUB(reportDate, INTERVAL $interval_hours HOUR) < 0", 
                        ['ip'=>$ip] );
	return ($a > $max_reports_per_interval);
}

function delete_and_adjust( $ip, $id )
{
	$db = new \SFACTIVE\DB();
	$db->execute("UPDATE qcrank SET score=score-0.5 WHERE ip=:ip", [':ip'=>$ip] );

	$article_obj = new \SFACTIVE\Article;
	$article_obj->update_article_status($id, 'f');

	$db->execute("DELETE FROM qc WHERE id=:id", [':ip'=>$ip]);
}

/**
 * @returns int modScore the moderator's moderation score.
 */
function report( $ip, $type, $id )
{
	$db = new SFACTIVE\DB();
	$modScore = $db->queryFetchOne("SELECT score FROM qcrank WHERE ip=:ip", [':ip'=>$ip]);
	if ($modScore==null) $modScore=0;


	// This increases the 'ranking' score on the article
	// The change to the score depends on the mod score of the moderator.
	// For every 10 mod points, they get an extra +1 boost to the article's rating.
	// Moderators with a negative mod score don't get to mod up articles.
	if ($type=='bestof')
	{
		$db->execute("UPDATE webcast SET rating=0 WHERE id=:id AND rating IS NULL", [':id'=>$id]);
		if ($modScore > 0) {
            $increment = log10($modScore) + 1;
        } else {
            $increment = 0;
        }
        $db->execute("UPDATE webcast SET rating=rating+:increment WHERE id=:id AND rating<10", [':id'=>$id, ':increment'=>$increment]);
		$db->execute("UPDATE webcast SET rating=10 WHERE rating>10");
		$article_obj = new Article;
		$article_obj->update_article_status($id, 't');
		return $modScore;
	}

    if ($type=='troll')
    {
        $affected = $db->update("UPDATE webcast SET rating=-1 WHERE id=$id AND rating IS NULL");
		if ($affected >= 1)
		{
			$article_obj = new Article;
			$article_obj->update_article_status($id, 't');
		}
		// probably should do a render_everything, cache_to_disk 
		// sequence instead....  but there should be a 
	    // simpler update_article method... ugh.
        return $modScore;
    }


	$isOk = $db->queryFetchOne("SELECT COUNT(*) as c FROM qcok WHERE id=:id", [':id'=>$id]);

	// If the article is "ok" and they report it, then you drop their
	// moderator score by one
	if ($isOk > 0) 
	{
		$db->insert("INSERT INTO qcrank (ip,score,lastIgnoreDate) VALUES (:ip,-1,NOW()) ON DUPLICATE KEY UPDATE score=score-1,lastIgnoreDate=NOW()", [':ip'=>$ip] );
		return $db->queryFetchOne("SELECT score FROM qcrank WHERE ip=:ip", [':ip'=>$ip] );
	}

	$db->insert( 'INSERT INTO qc (type,id,ip,reportDate) VALUES (:type,:id,:ip,NOW())', [':type'=>$type, ':id'=>$id, ':ip'=>$ip] );
	$db->insert( 'INSERT INTO qcrank (ip,score,lastIgnoreDate) VALUES (:ip,0,NOW()) ON DUPLICATE KEY UPDATE lastIgnoreDate=NOW()', [':ip'=>$ip] );

	return $modScore;
}

// The script starts here


// fixme - this should use POST, not GET.

$request = Request::createFromGlobals();

$id = $request->query->get('id');
if (!filter_var($id,FILTER_VALIDATE_INT)) { 
    throw new Exception('bad id'); 
}

$format = $request->query->get('format', 'html');
if (!in_array($format, ['html','json'])) {
    throw new Exception('format must be html or json'); 
}

$type = $request->query->get('q');
if (!preg_match('/(dmca|fraud|racist|genocide|chatter|porn|double|spam|ad|hate|offtopic|bestof|troll)/',$type) ) {
    throw new Exception('invalid type'); 
}

$callback = $request->query->get('callback');
if ($callback!==null && !preg_match('/^[a-zA-Z_][a-zA-Z0-9_]{0,99}/', $callback)) {
    throw new Exception('callback function names must be less than 100 characters long and use only letters, numbers and underscore.'); 
}

$ip = $request->cookies->get('snitchip', abs(crc32($_SERVER['REMOTE_ADDR'])));
if (!filter_var($ip,FILTER_VALIDATE_INT)) {
    throw new Exception('bad ip'); 
}


$db = new SFACTIVE\DB();
$out = ['ip'=>$ip];

if ( too_many_reports( $ip, 12, 15 ) )
{
    $out['message'] = "You can only submit 15 reports per 12 hour period.";
}
else
{
    $out['message'] = 'Thanks for helping us moderate.';

    $out['moderatorScore'] = $modScore = report( $ip, $type, $id );
    if ( ($modScore > 20) and  $type!='bestof' )
    {
        delete_and_adjust( $ip, $id );
    }
}

if ($format=='html') 
{
    $response = new Response( '', Response::HTTP_OK, ['content-type'=>'text/html'] );

	$template = <<<TEMPLATE
    <html>
        <body style="margin: 0px">
            <p style="margin: 4px;">
                <strong>Peer Moderation</strong>
                <br /><br />
                {{ message }}
                <small>ID: {{ ip }} <br>Your Moderator Score: {{ moderatorScore }}.</small>
            </p> 
            <p> 
                <img src="/qc/0.jpg"> 
            </p> 
            <script> 
                window.setTimeout( () => { window.close(); }, 6000 );
            </script>
        </body>
    </html>
TEMPLATE;

    $twig = new Twig_Environment(new Twig_Loader_Array(['page'=>$template]));
    $response->setContent($twig->render('page', $out));
}
else
{
    $response = new JsonResponse( $out );
    if ($callback) {
        $response->setCallback( $callback );
    } 
}

$response->prepare($request);
$response->headers->setCookie(new Cookie('snitchip',$ip, time()+60*60*24*2600));
$response->send();

