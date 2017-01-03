<!-- comment form -->

<h3>Guidelines for Commenting on News Articles</h3>

<p>Thanks for contributing to TPL_LOCAL_SITE_NICK's <a href="/process/openpub.php">open publishing</a> Newswire.</p>

<p>Your response article can be in any format, from academic discourse to subjective personal account.</p>

<p>Please keep it on topic and concise.</p>

<p>And please read our <a href="/process/privacy.php">privacy</a> and <a href="/process/disclaimer.php">legal</a> statements before continuing.</p>

<form enctype="multipart/form-data" method="POST" action="/publish.php">
<input type="hidden" name="top_id" value="TPL_LOCAL_TOP_ID">

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

<tr valign="top" class="bgpenult"> 

<td width="25%">
<strong>TPL_PUB_AUTHOR</strong> <small>(TPL_REQUIRED)</small>
</td>

<td width="75%">
<input type="text" name="author" size="25" maxlength="45" value="TPL_LOCAL_AUTHOR" />
</td>

</tr><tr class="bgpenult">

<td width="25%">
TPL_EMAIL
</td>

<td width="75%">
<input type="text" name="contact" size="25" maxlength="80" value="TPL_LOCAL_CONTACT" />
</td>

</tr><tr class="bgpenult">

<td width="25%">
TPL_PHONE
</td>

<td width="75%">
<input type="text" name="phone" size="25" maxlength="80" value="TPL_LOCAL_PHONE" />
</td>

</tr><tr class="bgpenult" valign="top">

<td width="25%">
TPL_ADDRESS
</td>

<td width="75%">
<input type="text" name="address" size="25" maxlength="160" value="TPL_LOCAL_ADDRESS">
<br />TPL_PUB_OPTIONAL
</td>

</tr>
<tr class="bgpenult">

<td width="25%">TPL_PUB_URL</td>

<td width="75%">
<input type="text" name="link" size="25" maxlength="160" value="TPL_LOCAL_LINK"/>
</td>

</tr></table>

</td></tr></table>

</td></tr></table>

<!-- comment form part 2 -->

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

<!-- comment form part 3 -->

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

<input type="submit" name="action" value="Enter">

</td>

</tr></table>
</td></tr></table>
</td></tr></table>

TPL_FILE_BOXES

</td>
</tr>
</table>
<p align="center"><input type="submit" name="publish" value="TPL_BUTTON_PUBLISH" /></p>
</form>
<!-- end comment template -->
