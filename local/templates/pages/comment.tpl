<!-- comment form -->

<h3>Guidelines for Commenting on News Articles</h3>

<p>Thanks for contributing to TPL_LOCAL_SITE_NICK's <a href="/process/openpub.php">open publishing</a> Newswire.</p>

<p>Your response article can be in any format, from academic discourse to subjective personal account.</p>

<p>Please keep it on topic and concise.</p>

<p>And please read our <a href="/process/privacy.php">privacy</a> and <a href="/process/disclaimer.php">legal</a> statements before continuing.</p>


<p style="background-color:pink">
<b>Please Don't Feed the Trolls</b>
:
Wikipedia defines an Internet Troll as: "either a person who sends messages on the Internet hoping to entice other users into angry or fruitless responses, or a message sent by such a person." Los Angeles IMC strives to provide both a grassroots media resource as well as a forum for people to contribute to a meaningful discussion about local issues. Please, when posting comments, be respectful of others and ignore those trying to interrupt or discourage meaningful discourse. Thank you.
</p>
<form enctype="multipart/form-data" method="POST" action="/publish.php" accept-charset="UTF-8">
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

<p align="center"><input type="submit" name="publish" value="TPL_BUTTON_PUBLISH" /></p>
</form>
<!-- end comment template -->
