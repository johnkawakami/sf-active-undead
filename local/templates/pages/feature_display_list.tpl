<!-- feature_display_list template -->

<A href="../index.php">TPL_ADMIN_INDEX</A> : 
<A href="../category/category_display_list.php">TPL_CATEGORY_LIST</A>
 : TPL_LOCAL_SUBTITLE
 &nbsp; &nbsp; | 
<A href="feature_display_edit.php">TPL_FEATURE_STORY_ADD</A>
 | 
<A href="/?preview=TPL_LOCAL_CATID">TPL_FEATURE_TEST</A>
 | 
<A href="../category/category_display_preview.php">TPL_FEATURE_PREVIEW</a>
<br /><br />
<B>TPL_STORY_INDEX_FOR <EM>TPL_LOCAL_CATEGORY_NAME</EM></B><br />
(TPL_STORY_INDEX_INST)

<FORM name="order_form" action="feature_reorder.php">

<TABLE><TR>

<TD>TPL_LOCAL_CURRENT_LINK</TD>
<TD>TPL_LOCAL_ARCHIVED_LINK</TD>
<TD>TPL_LOCAL_HIDDEN_LINK</TD>
</TR></TABLE>

<TABLE class="bgsearchgrey" width="100%" border=1 >
<tr>
<td><b>TPL_FEATURE_ID_NUMBER</b></td>
<td><b>TPL_TITLE</b></td>
<td><b>TPL_CREATED</b></td>
<td><b>TPL_MODIFIED</b></td>
<td><b>TPL_ORDER_NUM</td>
<td>&nbsp;</td>
<td>&nbsp;</td>
<td>&nbsp;</td>
</tr>
TPL_LOCAL_TABLE_ROWS

</TABLE>

<br />

<INPUT type=submit name="reorder" value="TPL_REORDER" />

</FORM>
