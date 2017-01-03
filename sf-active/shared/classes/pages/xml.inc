<?php
// This include file outputs dynamically-generated newswire listings in a
// "proprietary" XML format used by akb's media syndication service (audio 
// and video) which is picked up by http://radio.indymedia.org/ and 
// http://www.indymedia.org/projects.php3 - see 
// http://dc.indymedia.org/demandmedia/ for complete listings
// For more info email syndication@indymedia.org

// Tue Mar  2 11:41:47 CET 2004 - bakunin <bakunin@autistici.org> 
//	support for id_request.

class xml extends Page
{
	function newswire()
	{
		return 1;
	}

	function execute()
	{
		header("Content-type:text/xml");
		echo "<"."?"."xml version=\"1.0\" standalone=\"yes\""."?".">"; 
?>

<org name="<?=$GLOBALS['site_nick']?>" canonical_url="<?=SF_ROOT_URL?>/">

<?php
$db_obj=new DB();

$id_request = $_GET['id_request'];

if(!$id_request) {
	$limit=$_GET['limit'];
	$page=$_GET['page'];
	$category=$_GET['category'];
	$hidden=$_GET['hidden'];
	$display=$_GET['display'];
	$comments=$_GET['comments'];
	$medium=$_GET['medium'];
	$keyword=$_GET['keyword'];
	$author=$_GET['author'];
	$year=$_GET['year'];
	$month=$_GET['month'];
	$day=$_GET['day'];
	$sort=$_GET['sort'];
}

if (!$limit) { $limit = 10; }

$last=($page - 1);
$next=($page + 1);

$offset = ($page * $limit);

if ($category) { $query = "
	select 
		w.id as id,
  		date_format(w.created,'%W %d %b %Y') as date_entered,
		w.author as author,
		w.heading as heading,
		w.mime_type as mime_type,
		w.parent_id as parent_id,
		w.created as created,
		w.summary as summary,
		w.modified as modified,
		w.numcomment as numcomment,
		w.linked_file as linked_file 
	from 
		webcast w,
		catlink l
	where"; }
else { $query = "
	select 
		id,
		date_format(created,'%W %d %b %Y') as date_entered,
		author,
		heading,
		mime_type,
		parent_id,
		created,
		summary,
		modified,
		numcomment,
		linked_file 
	from 
		webcast 
	where"; } 

if ($id_request) {
	$query .= " id='$id_request'";

} else {
	if ($hidden) { $query .= " display='f'"; } 
	else { $query .= " display!='f'"; }

	if ($category) $query .= " and w.id=l.id and l.catid=$category";
	if ($display) $query .= " and display='$display'";
	if (!$comments) { $query .= " and parent_id=0"; }
	if ($medium == "all") $medium="";
	if ($medium) $query .= " and mime_type like '%$medium%'";
	if ($keyword) {
		$query .= " and (summary like '%$keyword%'";
		$query .= " or article like '%$keyword%'";
		$query .= " or heading like '%$keyword%'";
		$query .= " or author like '%$keyword%')"; }

	if ($author) $query .= " and author='$author'";
	if ($year) $query .= " and created like '" . $year . $month . $day . "%'";
	if ($sort=="modified") { $query .= " and numcomment>=1"; }
	if ($sort=="rank") { $query .= " ORDER BY numcomment DESC"; }
	else if ($sort=="modified") { $query .= " ORDER BY modified DESC"; }
	else if ($sort=="reverse") { $query .= " ORDER BY created ASC"; }
	else { $query .= " ORDER BY created DESC"; }
	$query .= " limit ".$offset."," . $limit;
}

$result = $db_obj->query($query);
$numrows= count($result);
if (is_array($result)) {
	for ($cell_counter=0; $cell_counter<$numrows; $cell_counter++) {
		$row = array_pop($result);
		$media_link="";
		
		if ($row[parent_id] != 0) {
			$query="SELECT created FROM webcast WHERE id=".$row[parent_id];
			$idcheck = $db_obj->query($query);
			$idrow=array_pop($idcheck);
			$searchlink=SF_NEWS_URL.'/'.MakeCacheDir($idrow[created]).$row[parent_id].".php";
			$searchlink.="#".$row[id];
			$idlink=$row[parent_id]."#".$row[id];
		
		} else {
			$searchlink=SF_NEWS_URL.'/'.MakeCacheDir($row[created]).$row[id].".php";
			$idlink=$row[id];
		} 
		
		$parent_id = $row["parent_id"];
		if ($parent_id=="0") { $parent_id=$row["id"]; $id=""; }
		
		$mime_type = $row["mime_type"];
		
		if ($row["linked_file"]) {
            		$filename = basename($row["linked_file"]);
			
			if (ereg("real.*o$",$row[mime_type])) { $media_link = $GLOBALS['rtsp_base_url'] . $filename; }
			else { $media_link = SF_UPLOAD_URL . "/" . $filename; }
		}

		$author = eregi_replace("&quot;","\"",$row[author]);
		$author = utf8_encode(htmlspecialchars(trim($author)));
		$author = eregi_replace("&amp;#","&#",$author);
		$heading = eregi_replace("&quot;","\"",$row[heading]);
		$heading = utf8_encode(htmlspecialchars(trim($heading)));
		$heading = eregi_replace("&amp;#","&#",$heading);
		$summary = eregi_replace("&quot;","\"",$row[summary]);
		$summary = utf8_encode(htmlspecialchars(trim($summary)));
		$summary = eregi_replace("&amp;#","&#",$summary);
		$date_entered = trim($row[date_entered]);
?>

<article id="<?=$row["id"]?>" 
date_entered="<?=$date_entered?>" 
linked_file="<?=$media_link?>" 
url="<?=$searchlink?>" mime_type="<?=$row[mime_type]?>" 
numcomment="<?=$row[numcomment]?>" author="<?=$author?>">
	<heading><?=$heading?></heading>
	<summary><?=$summary?></summary>
</article>
<?	
	}	
}	
?>

</org>
<?  
	} // function execute
} // class
?>
