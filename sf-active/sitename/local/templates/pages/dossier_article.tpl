<div align="left"><a href="index.php">TPL_DOSSIER_LIST_LINK</a></div>

<br /><div>
TPL_STATUS
</div><br />

<h2>TPL_SUB_TITLE</h2>

<b>TPL_HTML_WARNING</b><br /><br />


<form enctype="multipart/form-data" method="post" action="article.php">
TPL_HIDDENSETTINGS

<!-- inizio fase uno della pubblicazione -->
<table width="100%" border="0" cellspacing="0" cellpadding="1" class="bgaccent">
	<tr>
		<td class="bgaccent">
		<b>TPL_PUB_STEPONE</b>
		<table width="100%" border="0" cellspacing="0" cellpadding="4" class="bgpenult">
			<tr>
				<td>
					<table width="100%" border="0" cellspacing="2" cellpadding="4">
						<tr class="bgpenult" valign="top">
							<td width="25%">
								<b>TPL_TITLE</b> (TPL_REQUIRED)
							</td>
							<td width="75%">
								<input type="text" name="heading" size="25" maxlength="90" value="TPL_LOCAL_HEADING" />
							</td>
						</tr>
						<tr valign="top" class="bgpenult">
							TPL_LOCAL_SELECT_A_DOSSIER
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
                <td width="25%"><b>TPL_AUTHOR</b>
                  (TPL_REQUIRED)</td>
<td width="75%"><input type="text" name="author" size="25" maxlength="45" value="TPL_LOCAL_AUTHOR" /></td>
              </tr>
<!-- porzione specifica dei dossier -->
              <tr valign="top" class="bgpenult">
                <td width="25%"><b>TPL_LINK_TO_ID</b>
                  (TPL_REQUIRED)</td>
<td width="75%"><input type="text" name="parent_id_dossier" size="25" maxlength="45" value="TPL_LOCAL_PARENT_ID" /><br />
		TPL_LINK_TO_ID_PUB<br />
		</td>
              </tr>
              <tr valign="top" class="bgpenult">
                <td width="25%"><b>TPL_ORDER_NUM</b>
                  (TPL_REQUIRED)</td>
<td width="75%"><input type="text" name="order_num" size="25" maxlength="45" value="TPL_LOCAL_ORDER_NUM" /><br />
		TPL_ORDER_NUM_PUB<br />
		</td>
              </tr>
<!-- fine parte specifica dossier -->
<tr valign="top" class="bgpenult">
                <td width="25%"><b>TPL_SUMMARY</b> (TPL_REQUIRED)</td>
                <td width="75%"><textarea name="summary" rows="3" cols="50" wrap="virtual">TPL_LOCAL_SUMMARY</textarea><br />TPL_PUB_SUM</td>
              </tr>
            </table></td>
        </tr>
      </table></td>
  </tr>
</table>
<!-- fine parte uno publish form, decurtata di contatti/telefono/indirizzo/link che non ci serve -->

<!-- inizio parte due della form di pubblicazione -->
<table width="100%" border="0" cellspacing="0" cellpadding="1" class="bgaccent">
  <tr>
    <td class="bgaccent"><b>TPL_PUB_STEPTWO</b>
      <table width="100%" border="0" cellspacing="0" cellpadding="4"  class="bgpenult">
        <tr>
          <td><table width="100%" border="0" cellspacing="2" cellpadding="4">
              <tr>
                <td width="25%" valign="top"><b>TEXT/HTML</b></td>
                <td width="75%"><textarea name="article" rows="10" cols="60" wrap=virtual>TPL_LOCAL_ARTICLE</textarea><br /></td>
              </tr>
		<input type="hidden" name="texttype" value="text/html" />
		<input type="hidden" name="artmime" value="h" />
            </table></td>
        </tr>
      </table></td>
  </tr>
</table>
<!-- fine parte due della form di pubblicazione -->

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

<input type="submit" name="action" value="Enter" /><br />
TPL_MAX_UPLOAD TPL_UPLOAD_MAX_FILESIZE; TPL_TOTAL_LIMIT TPL_POST_MAX_SIZE; TPL_MAX_EXECUTION_TIME TPL_MINUTES
<br />TPL_ACCEPTED_TYPES

</td></tr></table>
</td></tr></table>
</td></tr></table>

TPL_FILE_BOXES

</td>
</tr>
</table>
<p align="center"><input type="submit" name="publish" value="TPL_BUTTON_PUBLISH" /></p>

</form>


<br /><br /><div align="left"><a href="index.php">TPL_DOSSIER_LIST_LINK</a></div>
