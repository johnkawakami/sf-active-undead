<table cellpadding="0" cellspacing="0" border="0">
	<tr>
	<form name="searchform" onsubmit="if (document.searchform.keyword.value=='TPL_SEARCH_BY_KEYWORD') document.searchform.keyword.value='';" action="/archives/" method="get">
		<td valign="top">
			<big><b>TPL_SITE_CRUMB TPL_ARCHIVE_INDEX</b></big> 
			<input type="hidden" name="current" value="TPL_CURRENT_SET" />
			<input style="float: right" type="submit" /><input style="float: right" type="text" name="keyword" size="7" value="TPL_KEYWORD_SET" onfocus="if (document.searchform.keyword.value=='TPL_SEARCH_BY_KEYWORD') document.searchform.keyword.value='';" />
			<center style="clear: right">TPL_LAST_LINK TPL_PAGE TPL_NEXT_LINK</center>
			
			TPL_LATEST

			<center>TPL_LAST_LINK TPL_PAGE TPL_NEXT_LINK</center>

			TPL_CURRENT_LINK
		</td>
	</form>
		<td>
			<big>&nbsp;</big>
		</td>
		<td class="bgsearchgrey">
			<big>&nbsp;</big>
		</td>
		<td class="bgsearchgrey" valign="top">
			TPL_CONTENTS
		</td>
	</tr>
</table>
