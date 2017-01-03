<a name="FEATUREID"></a><strong><big>TITLE2</big></strong><br />SUMMARY

<? 	require_once("../../shared/classes/rdf_class.php");
    require_once("../../shared/classes/rss_fetch.inc");
    require_once("../../shared/classes/rss_parse.inc");
    require_once("../../shared/classes/rss_cache.inc");
    require_once("../../shared/classes/rss_utils.inc");

function show_rdf_in_features( $filename, $title, $url, $count )
{
	$rdf = new rdfFile("$filename");
	$rdf->parse(True);
	print "<p><b>$title</b><br /><small>\n";
	for ( $c=0; $c<$count; $c++) {
		$desc = $rdf->Items[$c]['Description'];
		$desc = preg_replace('/llt;/','<', $desc);
	    print "* <a href=$url>" . $desc . "</a><br />\n";
	}
	print "</small></p>\n";
}

function show_rss_in_features( $url, $title, $url, $count )
{
	$rss = fetch_rss( $url );
	$ar = array_slice( $rss, 0, $count );
	print "<p><b>$title</b><br /><small>\n";
	foreach ( $ar as $item ) {
		$desc = $item['title'];
		$desc = preg_replace('/llt;/','<', $desc);
	    print "* <a href=$url>" . $desc . "</a><br />\n";
	}
	print "</small></p>\n";
}
// warning - this fucntion throws warnings and errors

?>
<br />
<table border=0 cellpadding=0 cellspacing=4><tr><td valign=top>

<? show_rdf_in_features("/home/httpd/la/website/syn/Labor_features.rdf",
   "Labor", "features/Labor/", 2); ?>
<? show_rdf_in_features("/home/httpd/la/website/syn/Art_features.rdf",
   "Art", "features/Art/", 2); ?>
<? show_rdf_in_features("/home/httpd/la/website/syn/Perpetual_War_features.rdf",
   "War", "features/Perpetual_War/", 2); ?>
<? show_rdf_in_features("/home/httpd/la/website/syn/Radio_IMC-LA_features.rdf",
   "Radio IMC-LA", "features/Radio_IMC-LA/", 2); ?>
<? show_rdf_in_features("/home/httpd/la/website/syn/Media_features.rdf",
   "Media", "features/Media/", 2); ?>

</td><td valign=top>

<? show_rdf_in_features("/home/httpd/la/website/syn/Elections_features.rdf",
   "Elections", "features/Elections/", 2); ?>
<? show_rdf_in_features("/home/httpd/la/website/syn/Environment_features.rdf",
   "Environment", "features/Environment/", 2); ?>
<? show_rdf_in_features("/home/httpd/la/website/syn/Neoliberalism_features.rdf",
   "Neoliberalism", "features/Neoliberalism/", 2); ?>
<? show_rdf_in_features("/home/httpd/la/website/syn/Social_Programs_features.rdf",
   "Social Programs", "features/Social_Programs/", 2); ?>
<? show_rdf_in_features("/home/httpd/la/website/syn/Raise_The_Fist_features.rdf",
   "Raise The Fist", "features/Raise_The_Fist/", 2); ?>
<? // show_rss_in_features("http://www.indymedia.org/main-features.rss",
   // "Global Indymedia", "http://www.indymedia.org", 4); ?>

</td></tr>
<tr><td colspan=2>
<p><b><a href=/calendar>Future Events from the Calendar</a></b><br />
<table border=0 cellpadding=0 cellspacing=1>
<? 
	$cal = new rdfFile("/home/httpd/la/website/syn/calendar.rdf");
	$cal->parse(True);
	for ( $i=0; $title = $cal->Items[$i]['Title']; $i++ )
	{
			$title = preg_replace('/qquot;/', '"', $title);
			$title = preg_replace('/aamp;/', '&amp;', $title);
			if ( preg_match( '/IMC/', $title ) ) { $title = "<b>$title</b>"; }
			if (! ($i % 2)) { print "<tr>"; }
			print "<td width=50% valign=top><small>".$title."</small></td>";  
			if ($i % 2) { print "</tr>\n"; }
	} 
?>
</table>
</p>
</td></tr></table>

<br clear="all" /><br />
