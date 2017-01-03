<table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr><td width="80%">
<strong>TPL_MAILING_LIST_ADMIN</strong>
</td>

<td width="20%"><a href="/admin/">TPL_ADMIN_INDEX</a></td>
</tr>
</table>

TPL_LOCAL_SUCCESS

<form name="mailinglist" method="post" action="admin_email.php">
<b>TPL_DATE:</b> TPL_LOCAL_NICEDATE

<br /><br />

<b>TPL_SUBJECT:</b><br />
<input type="hidden" name="action" value="send" />
<input type="text" name="msubject" value="TPL_LOCAL_MSUBJECT" size="40" />

<br /><br />

<b>TPL_MESSAGE_BODY:</b>
<br />

<textarea name="mbody" rows="24" cols="70">
TPL_LOCAL_TEXTAREA
</textarea>

<br /><br />

TPL_CHECK_HERE: <input type="checkbox" name="failsafe" value="1" />

<br /><br />

<input type="submit" name="Submit" value="TPL_SEND" />
<br />
TPL_LIST_RECIPIENTS: TPL_LOCAL_SENDTO_ADDRESS
</form>
