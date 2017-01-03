<!-- Mailing List Template -->

<form action="mailinglist.php" method="post">

	TPL_UPDATE

<h3>TPL_WEEKLY_EMAIL</h3>

<p>TPL_TOP_TEXT</p>

<table>
<tr>
<th align="left">TPL_EMAIL_ADDRESS:</th>
<td><input type="text" name="email" value="" /></td>
</tr>

<tr>
<td></td>
<td align="right"><input type="submit" name="add_email" value="TPL_SUBSCRIBE" /></td>
</tr>

<tr>
<td></td>
<td align="right"><input type="submit" name="remove_email" value="TPL_UNSUBSCRIBE" /></td>
</tr>

<tr>
<td></td>
<td align="right"><input type="submit" name="remind_email" value="TPL_EMAIL_PASS" /></td>
</tr>

</table>

<h3>TPL_XML_NEWSFEEDS</h3>

<p>TPL_XML_TEXT:
<ul>
<li><a href="/syn/features.rdf">TPL_FEATURE_STORIES</a> (rdf)</li>
<li><a href="/newswire.rss">TPL_OPEN_NEWSWIRE</a> (rdf)</li>
<li><a href="/newswire.xml">TPL_OPEN_NEWSWIRE</a> (backslash)</li>
<li><a href="/syn/">TPL_SYNDICATION_PAGE</a></li>

<!-- commented out, seems not to work anymore
<li><a href="/news/?output=xml">CUSTOM_XML</a></li>
-->
</ul>

</form>

<!-- End Mailing List Template -->
