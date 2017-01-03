<!-- template for feature_display_edit -->

<a href="../index.php">TPL_ADMIN_INDEX</a>
 : 
<a href="/admin/category/category_display_list.php">TPL_CATEGORY_LIST</a>
 :
<a href="feature_display_list.php">TPL_FEATURE_LIST</a>
 : TPL_LOCAL_SUBTITLE &nbsp; &nbsp;
 TPL_LOCAL_HISTORY_LINK

<form name="TPL_LOCAL_FORM_NAME" action="TPL_LOCAL_FORM_ACTION" method="post">
<INPUT type="submit"  name="TPL_LOCAL_FORM_SHORT_ACTION" value="TPL_LOCAL_FORM_SHORT_ACTION" />
TPL_LOCAL_COPY_LINK	

<BR />
<TABLE cellspacing=5>

TPL_LOCAL_UPDATE_ROWS

<TR>
<TD>TPL_STATUS</TD>
<TD>TPL_LOCAL_SELECT_STATUS</TD>
</TR>

<tr>
    <td>TPL_LANGUAGE</td>
    <td>TPL_LOCAL_LANGUAGE</td>
</tr>

<TR>
<TD><b>TPL_FEATURE_TITLE1</b></TD>
<TD><INPUT NAME="title1" VALUE="TPL_LOCAL_TITLE1" /></TD>
</TR>

<TR>
<TD>TPL_FEATURE_TITLE2</TD>
<TD><INPUT size=60 NAME="title2" VALUE="TPL_LOCAL_TITLE2" /></TD>
</TR>

<TR>
<TD>TPL_TEMPLATE_NAME</TD>
<TD>TPL_LOCAL_SELECT_TEMPLATE</TD>
</TR>

<TR>
<TD><b>TPL_ORDER_NUM</b></TD>
<TD><INPUT NAME="order_num" VALUE="TPL_LOCAL_ORDER_NUM" /></TD>
</TR>

<TR>
<TD>TPL_FEATURE_DISPLAY_DATE</TD>
<TD><INPUT NAME="display_date" VALUE="TPL_LOCAL_DISPLAY_DATE"></TD>
</TR>

<TR>
<TD>TPL_CREATED</TD>
<TD>TPL_LOCAL_SELECT_DATE</TD></TR>

<TR><TD>TPL_FEATURE_IMAGE_URL</TD>
<TD><INPUT size=60 NAME="image" VALUE="TPL_LOCAL_IMAGE" /></TD>
</TR>

<TR>
<TD>TPL_FEATURE_ALT_TAG</TD>
<TD><INPUT size=60 NAME="tag" VALUE="TPL_LOCAL_TAG" /></TD>
</TR>

<TR>
<TD>TPL_LINK</TD>
<TD><INPUT size=60 NAME="image_link" VALUE="TPL_LOCAL_IMG_LINK" /></TD>
</TR>

<TR>
<TD colspan="2">TPL_SUMMARY</TD>
</TR>

<TR>
<TD colspan="3">
<TEXTAREA NAME="summary" cols=80 rows=20>TPL_LOCAL_SUMMARY</TEXTAREA></TD>
</TR>
</TABLE>

<INPUT type="submit"  name="TPL_LOCAL_FORM_SHORT_ACTION" value="TPL_LOCAL_FORM_SHORT_ACTION" />

</FORM>

<!-- end template -->
