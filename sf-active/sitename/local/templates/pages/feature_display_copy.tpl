<!-- template for feature_display_edit -->

<a href="../index.php">TPL_ADMIN_INDEX</a>
 : 
<a href="/admin/category/category_display_list.php">TPL_CATEGORY_LIST</a>
 :
<a href="feature_display_list.php">TPL_FEATURE_LIST</a> : TPL_LOCALSUBTITLE &nbsp; &nbsp; TPL_LOCAL_HISTORY_LINK

<form name="TPL_LOCAL_FORM_NAME" action="TPL_LOCAL_FORM_ACTION" method="post">
TPL_FEATURE_COPY_BLURB
<br>
TPL_CATEGORY_SELECT
<br />
<INPUT type="submit"  name="copy" value="TPL_COPY_ACTION" />	

<INPUT type="hidden" NAME="title1" VALUE="TPL_LOCAL_TITLE1"/>
<INPUT type="hidden" NAME="title2" VALUE="TPL_LOCAL_TITLE2" />
<INPUT type="hidden" NAME="template_name" VALUE="TPL_LOCAL_TEMPLATE" />
<INPUT type="hidden" NAME="order_num" VALUE="TPL_LOCAL_ORDER_NUM" />
<INPUT type="hidden" NAME="display_date" VALUE="TPL_LOCAL_DISPLAY_DATE" >
<INPUT type="hidden" NAME="image" VALUE="TPL_LOCAL_IMAGE" />
<INPUT type="hidden" NAME="tag" VALUE="TPL_LOCAL_TAG" />
<INPUT type="hidden" NAME="image_link" VALUE="TPL_LOCAL_IMG_LINK" />
<INPUT type="hidden" NAME="summary" value="TPL_LOCAL_SUMMARY" />
<input type="hidden" name="language_id" value="TPL_LOCAL_LANGUAGE_ID" />

</FORM>

<!-- end template -->
