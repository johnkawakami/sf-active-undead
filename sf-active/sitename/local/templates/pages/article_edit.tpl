
<!-- article_edit template -->

<a href="../index.php">TPL_ADMIN_INDEX</a> : 
<a href="./index.php">TPL_ADMIN_ARTICLE</a> : 
TPL_EDIT | <a href="article_regenerate.php">TPL_REGEN_ARTICLE</a>


<form action="article_edit.php" method="POST">

<input type="hidden" name="editswitch" value="1" />

<input type="hidden" name="id" value="TPL_LOCAL_ID" />

<table border="0" cellspacing="1" cellpadding="1" class="bgsearchgrey">
<tr><td colspan="2"><input type=submit name="save" value="save" />
TPL_PUBLISH_RESULT
</td></tr>

<tr><td>TPL_ID:</td>
<td>TPL_LOCAL_ID</td></tr>

<tr><td>TPL_CREATED:</td>
<td>TPL_LOCAL_CREATED</td></tr>

<tr><td>TPL_MODIFIED:</td>
<td>TPL_LOCAL_MODIFIED</td></tr>

<tr><td>TPL_TITLE:</td>
<td><input type="text" name="heading" size="70" value="TPL_LOCAL_TITLE" /></td></tr>

<tr><td>TPL_AUTHOR:</td>
<td><input type="text" name="author" size="70" value="TPL_LOCAL_AUTHOR" /></td></tr>

<tr><td>TPL_EMAIL:</td>
<td><input type="text" name="contact" size="70" value="TPL_LOCAL_CONTACT" /></td></tr>

<tr><td>TPL_LINK:</td>
<td><input type="text" name="link" size="70" value="TPL_LOCAL_LINK" /></td></tr>

<tr><td>TPL_ADDRESS:</td>
<td><input type="text" name="address" size="70" value="TPL_LOCAL_ADDRESS" /></td></tr>

<tr><td>TPL_PHONE:</td>
<td><input type="text" name="phone" size="70" value="TPL_LOCAL_PHONE" /></td></tr>

<tr><td>TPL_CATEGORY_LIST:</td>
<td>
TPL_LOCAL_CHECKBOX
</td></tr>

<tr><td>TPL_SEL_LANGUAGE:</td>
<td>TPL_DROPDOWN_LANGUAGE
</td></tr>

<tr><td>TPL_MIMETYPE:</td>
<td>
TPL_SELECT_MIME
</td></tr>

<tr><td>TPL_TEXTMIME:</td>
<td>
TPL_SELECT_TEXT
</td></tr>

<tr><td>TPL_ARTTYPE:</td>
<td>
TPL_SELECT_ARTTYPE
</td></tr>

<tr><td>TPL_DISPLAYSTAT:</td>
<td>
TPL_SELECT_DISPLAY
</td></tr>

<tr><td>TPL_REASON:</td>
<td>
TPL_SELECT_REASON
</td></tr>

<tr><td>TPL_ADDITIONAL_REASON:</td>
<td>
<input type="text" name="additional_reason" value="TPL_ADDITIONAL_REASON_VALUE">
</td></tr>

<input type="hidden" name="old_display" value="TPL_LOCAL_OLD_DISPLAY">

<tr><td>TPL_PARENT_ID:</td>
<td><input type="text" name="parent_id" size="10" value="TPL_LOCAL_PARENTID" /></td></tr>

<tr><td>TPL_MEDIAFILE:</td>
<td><input type="text" name="linked_file" value="TPL_LOCAL_FILE" /></td></tr>

<tr><td colspan="2">TPL_SUMMARY:<br />
<textarea name="summary" cols="80" rows="5">TPL_LOCAL_SUMMARY</textarea></td></tr>

<tr><td colspan="2">TPL_ARTICLE:<br>
<textarea name="article" cols="80" rows="20">TPL_LOCAL_ARTICLE</textarea></td></tr>

<tr><td colspan="2"><input type=submit name="save2" value="save"></td></tr></table>
<input type="hidden" name="timestamp" value="TPL_LOCAL_MICROTIME">
</form>

<!-- end article_edit template -->

