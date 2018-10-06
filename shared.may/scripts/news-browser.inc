<?php
$db_obj=new DB();
$url_author=urlencode($author);
$url_keyword=urlencode($keyword);
if (!$limit) $limit = $GLOBALS['default_search_size'];
$last=($page - 1);
$next=($page + 1);
if ($page<0) $page=0;
$offset = ($page * $limit);
if ($category) {
	$query = "select w.display as display,w.id as id,date_format((w.created + interval         ".$time_diff." second), '%Y-%m-%d  %h:%i %p') as date_entered,w.author as author,w.heading as heading,w.mime_type as mime_type,w.parent_id as parent_id,w.created as created, date_format(w.created, '%Y') as created_year, date_format(w.created, '%m') as created_month, w.summary as summary,w.modified as modified,w.numcomment as numcomment,w.arttype as arttype from webcast w,catlink l where";
} else {
	$query = "select display,id,date_format((created + interval         ".$time_diff." second), '%Y-%m-%d  %h:%i %p') as date_entered,author,heading,mime_type,parent_id,created, date_format(created, '%Y') as created_year, date_format(created, '%m') as created_month, summary,modified,numcomment,arttype from webcast where";
}
if ($hidden) {
	$query .= " display='f'";
} else if ($display) {
	$query .= " display='".$display."'";
} else if ($edit==1) {
	$query .= " display like '%'";
} else {
	$query .= " display!='f'";
}
if ($category) $query .= " and w.id=l.id and l.catid=$category";
if (!$comments) $query .= " and parent_id=0";
if ($author) $query .= " and author='$author'";
if ($medium == "all") $medium="";
if ($medium) $query .= " and mime_type like '%$medium%'";
if ($keyword) {
	$query .= " and (summary like '%$keyword%'";
	$query .= " or article like '%$keyword%'";
	$query .= " or heading like '%$keyword%'";
	$query .= " or author like '%$keyword%')";
}
if ($year) $query .= " and created like '" . $year . $month . $day . "%'";
if ($sort=="modified") $query .= " and numcomment>=1";
if ($sort=="rank") {
	$query .= " ORDER BY numcomment DESC";
} else if ($sort=="modified") {
	$query .= " ORDER BY modified DESC";
} else if ($sort=="reverse") {
	$query .= " ORDER BY created ASC";
} else {
	$query .= " ORDER BY created DESC";
}
$query .= " limit ".$offset."," . $limit;
$result = $db_obj->query($query);
if ($result) $numrows=count($result);
if ($numrows or $page) include("$shared_scripts_path/news-browser-nav.inc");
if ($hidden) echo "<h3>$lang_hidden_warning</h3>";
if (($month and !$year) or ($day and !$month)) echo "<h3>$lang_date_warning</h3>";
if ($numrows == 0) {
	echo "<h2>$lang_no_results";
	if (ereg(" ",$keyword)) echo "<h4>$lang_search_suggestion</h4>";
	echo "</h2>"; include("$local_include_path/search-form.inc");
}

if ($numrows > 0) {
	if ($edit==1) {
		?><form method="Post"><input type="submit" name="Save" value="Save" /><?
	}
	for ($cell_counter=0; $cell_counter<$numrows; $cell_counter++) {
		$row = array_pop($result);
		if ($row[parent_id] != 0) {
			if ($row[display]=="f") {
				$searchlink="/news/hidden.php?id=$row[parent_id]#$row[id]";
			} else {
				$query="SELECT created FROM webcast WHERE id=".$row[parent_id];
				$idcheck = $db_obj->query($query);
				$idrow=array_pop($idcheck);
				$cachefile=$webroot_news_path."/".MakeCacheDirYearMonth($idrow[created_year],$idrow[created_month]).$row[parent_id]."_comment.php";
				$searchlink="/news/".MakeCacheDir($idrow[created]).$row[parent_id]."_comment.php";
				$searchlink.="#".$row[id];
				$idlink=$row[parent_id]."#".$row[id];
			}
		} else {
			$cachefile=$webroot_news_path."/".MakeCacheDir($row[created]).$row[id].".php";
			$searchlink="/news/".MakeCacheDir($row[created]).$row[id].".php";
			$idlink=$row[id];
		}
		$parent_id=$row[parent_id];
		$search=array("'\''","'<'","'>'");
		$replace=array("&#039;","&lt;","&gt;");
		$heading=preg_replace($search,$replace,$row[heading]);
		$author=preg_replace($search,$replace,$row[author]);
		$summary=preg_replace($search,$replace,$row[summary]);
		if ($parent_id=="0") {
			$parent_id=$row[id]; $id="";
		}
		$mime_type = $row[mime_type];
		if (ereg("text",$mime_type)) $mime='text" src="/im/imc_article.gif" ';
		if (strlen($mime_type)<1) $mime='unknown" src="/im/imc_article.gif" ';
		if (ereg("image",$mime_type)) $mime='image" src="/im/imc_photo.gif" ';
		if (ereg("audio",$mime_type)) $mime='audio" src="/im/imc_audio.gif" ';
		if (ereg("video",$mime_type)) $mime='video" src="/im/imc_video.gif" ';
		if (ereg("application",$mime_type)) $mime='application" src="/im/imc_pdf.gif" ';
		if ($row[arttype]=="news-response") $mime='comment" src="/im/imc_comment.gif" ';
		if ($edit==1) {
			include("$shared_scripts_path/news-browser-admin.inc"); 
		} else {
			include("$shared_scripts_path/news-browser-display.inc");
		}
	}
	if ($edit==1) {
		?><input type="hidden" name="numsubmittedrows" value="<?=$rownum?>" /><br /><input type="submit" name="Save" value="Save" /></form><?
	}
	if ($numrows or $page) include("$shared_scripts_path/news-browser-nav.inc");
}
?>
