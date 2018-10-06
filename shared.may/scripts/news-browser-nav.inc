<p><table cellpadding="0" cellspacing="0" border="0" width="100%"><tr><td><?php
if ($page) echo "<a href=\"$SCRIPT_NAME?author=$url_author&amp;category=$category&amp;comments=$comments&amp;day=$day&amp;display=$display&amp;hidden=$hidden&amp;keyword=$url_keyword&amp;limit=$limit&amp;medium=$medium&amp;month=$month&amp;page=$last&amp;sort=$sort&amp;year=$year\">&lt;&lt;$lang_newer_stories</a>";
?></td><td align="center"><?
if ($numrows) {
	if ($include_form) {
		include("$local_include_path/news-browser-form.inc");
	} else {
		echo "<a href=\"/news/search/?author=$url_author&amp;category=$category&amp;comments=$comments&amp;day=$day&amp;display=$display&amp;hidden=$hidden&amp;keyword=$url_keyword&amp;limit=$limit&amp;medium=$medium&amp;month=$month&amp;page=$page&amp;sort=$sort&amp;year=$year\">$lang_advanced_browse</a>";
	}
}
?></td><td align="right"><?
if ($numrows == $limit) echo "<a href=\"$SCRIPT_NAME?author=$url_author&amp;category=$category&amp;comments=$comments&amp;day=$day&amp;display=$display&amp;hidden=$hidden&amp;keyword=$url_keyword&amp;limit=$limit&amp;medium=$medium&amp;month=$month&amp;page=$next&amp;sort=$sort&amp;year=$year\">$lang_more_stories&gt;&gt;</a>";
?></td></tr></table></p>
