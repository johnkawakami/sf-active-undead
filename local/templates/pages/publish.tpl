<!-- publish form part 1 -->

TPL_DISPLAY_PREVIEW

<a name="publishform"></a>
<form id="theform" enctype="multipart/form-data" method="POST" action="/publish.php" accept-charset="UTF-8">

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
</tr>
<tr valign="top" class="bgpenult"> 
		<td width="25%">
				Secret 
		</td>
		<td width="75%">
				<input type="text" name="secret" size="25" maxlength="45" value="" /><br />
				<small>
					The secret is used to produce an id verification code displayed with your author name.
					Use the same author name and secret on all your posts.
				</small>
		</td>
</tr>

<tr class="bgpenult">

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

<div class="bgpenult" style="color: white; background-color: #909090; padding: 0px 0px 0px 4px; margin: 0px;">
<strong>STEP 4: HUMAN-NESS</strong>
</div>
<p style="border:1px solid #909090; padding: 10px; margin-top: 0px;">
To convince us that you're a human being and not a spambot, please
answer the following math question.  <br />
Type the answer in English:
<b>What is TPL_CAPTCHA_QUESTION? </b>
<input type="text" name="craptcha" />
<input type="hidden" name="csrf_token" value="TPL_CSRF_TOKEN" />
</p>

<div id="submitbuttons">
		<p align="center">TPL_PREVIEW_WARNING<br /><br /><input type="submit" name="preview"
		value="TPL_PREVIEW" /><input type="submit" name="publish" value="TPL_BUTTON_PUBLISH" onclick="subm()" /></p>
</div>
<div id="progressbar" style="text-align: center; display:none;">
    <div style="border: 1px solid red; padding: 5px; width: 200px; margin: 2px auto 2px auto;">
    <div>Uploading: please give it some time.</div>
    <img src="/im/progress.gif" />
</div>
</form>
<script type="text/javascript">
function subm() {
  var submitbuttons = document.getElementById('submitbuttons');
  var sub = document.getElementById('sub');
  var progressbar = document.getElementById('progressbar');
  var theform = document.getElementById('theform');

  disableForm(theform);
  submitbuttons.style.display = "none";
  progressbar.style.display = "block";
  return true;
}
function disableForm(theform) {
  if (document.all || document.getElementById) {
    for (i = 0; i < theform.length; i++) {
      var formElement = theform.elements[i];
      // if(formElement.getAttribute('type')=='text') formElement.disabled = true;
      // if(formElement.getAttribute('type')=='file') formElement.disabled = true;
      // if(formElement.getAttribute('type')=='textarea') formElement.disabled = true;
    }
  }
}

</script>

<!-- end publish template -->
