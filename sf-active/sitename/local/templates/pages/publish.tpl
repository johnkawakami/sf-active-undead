<!-- publish form part 1 -->

TPL_DISPLAY_PREVIEW

<a name="publishform"></a>
<form enctype="multipart/form-data" method="POST" action="/publish.php">

<table width="100%" border="0" cellspacing="0" cellpadding="1" class="bgaccent">
<tr><td class="bgaccent">

<strong>&nbsp;TPL_PUB_STEPONE</strong>		

<table width="100%" border="0" cellspacing="0" cellpadding="4" class="bgpenult">
<tr><td>				

<table width="100%" border="0" cellspacing="2" cellpadding="4">
<tr class="bgpenult" valign="top">

<td width="25%">
<strong>TPL_TITLE</strong> <small>(TPL_REQUIRED)</small>
</td>

<td width="75%">
<input type="text" name="heading" size="25" maxlength="90" value="TPL_LOCAL_HEADING" />
</td>

</tr>

<tr>
  <td width="25%">
    <strong>TPL_SEL_LANGUAGE</strong> <small>(TPL_REQUIRED)</small>
  </td>
  <td width="75%">
    TPL_DROPDOWN_LANGUAGE
  </td>
</tr>


TPL_DROPDOWN_CAT


<tr valign="top" class="bgpenult"> 

<td width="25%">
<strong>TPL_PUB_AUTHOR</strong> <small>(TPL_REQUIRED)</small>
</td>

<td width="75%">
<input type="text" name="author" size="25" maxlength="45" value="TPL_LOCAL_AUTHOR" />
</td>

</tr><tr class="bgpenult">

<td width="25%">TPL_EMAIL</td>

<td width="75%">
<input type="text" name="contact" size="25" maxlength="80" value="TPL_LOCAL_CONTACT" />
</td>
</tr>

<!-- new part for e-mail validation code - by jmh/mtoups -->
<tr class="bgpenult">
    <td width="25%">TPL_VALIDATE_EMAIL</td>
    <td width="75%">
	TPL_VALIDATE_CHECKBOX 
	TPL_VALIDATE_FORM
    </td>
</tr>
<!-- end e-mail validation - jmh/mtoups -->


<tr class="bgpenult">

<td width="25%">TPL_PHONE</td>

<td width="75%">
<input type="text" name="phone" size="25" maxlength="80" value="TPL_LOCAL_PHONE" />
</td>

</tr><tr class="bgpenult" valign="top">

<td width="25%">TPL_ADDRESS</td>

<td width="75%">
<input type="text" name="address" size="25" maxlength="160" value="TPL_LOCAL_ADDRESS">
<br />TPL_PUB_OPTIONAL
</td>

</tr><tr valign="top" class="bgpenult">

<td width="25%">
<strong>TPL_SUMMARY</strong> <small>(TPL_REQUIRED)</small>
</td>

<td width="75%">
<textarea name="summary" rows="3" cols="50" wrap="virtual">TPL_LOCAL_SUMMARY</textarea>
<br />TPL_PUB_SUM
</td>

</tr><tr class="bgpenult">

<td width="25%">TPL_PUB_URL</td>

<td width="75%">
<input type="text" name="link" size="25" maxlength="160" value="TPL_LOCAL_LINK"/>
</td>

</tr></table>

</td></tr></table>

</td></tr></table>

<!-- publish form part 2 -->

<table width="100%" border="0" cellspacing="0" cellpadding="1" class="bgaccent">
<tr><td class="bgaccent">
<strong>&nbsp;TPL_PUB_STEPTWO</strong>

<table width="100%" border="0" cellspacing="0" cellpadding="4"  class="bgpenult">
<tr><td>

<table width="100%" border="0" cellspacing="2" cellpadding="4">
<tr>

<td width="25%" valign="top">
<strong>TEXT/HTML</strong>
</td>

<td width="75%">
<textarea name="article" rows="10" cols="60" wrap=virtual>TPL_LOCAL_ARTICLE</textarea>
<br />TPL_PUB_ART
</td>

</tr><tr valign="top">

<td width="25%">
<strong>TPL_PUB_ARTMIME</strong>
</td>


<td width="75%">
TPL_SELECT_ARTMIME
<br />
TPL_PUB_MIME

</td>

</tr></table>

</td></tr></table>

</td></tr></table>

<!-- publish form part 3 -->

<table width="100%" border="0" cellspacing="0" cellpadding="1" class="bgaccent">
<tr><td class="bgaccent">
<strong>&nbsp;TPL_PUB_STEPTHREE</strong>

<table width="100%" border="0" cellspacing="0" cellpadding="1" class="bgaccent">
<tr class="bgpenult"><td>

<table width="100%" border="0" cellspacing="0" cellpadding="4" class="bgpenult">
<tr><td>

<table width="100%" border="0" cellspacing="2" cellpadding="4">
<tr class="bgpenult" valign="top">

<td width="25%">
<strong>TPL_PUB_CUANTOFILES</strong>
</td>

<td width="75%">
TPL_SELECT_FILECOUNT

<input type="submit" name="action" value="Enter"><br />
TPL_MAX_UPLOAD TPL_UPLOAD_MAX_FILESIZE; TPL_TOTAL_LIMIT TPL_POST_MAX_SIZE; TPL_MAX_EXECUTION_TIME TPL_MINUTES
<br />TPL_ACCEPTED_TYPES
</td>

</tr></table>
</td></tr></table>
</td></tr></table>

TPL_FILE_BOXES

</td>
</tr>
</table>
<p align="center">TPL_PREVIEW_WARNING<br /><br /><input type="submit" name="preview"
value="TPL_PREVIEW" /><input type="submit" name="publish" value="TPL_BUTTON_PUBLISH" /></p>
</form>
<!-- end publish template -->
