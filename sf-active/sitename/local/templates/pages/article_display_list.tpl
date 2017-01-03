<A href="../index.php">TPL_ADMIN_INDEX</A> : 
TPL_ARTICLES_EDIT_LIST | <a href="article_regenerate.php">TPL_REGEN_ARTICLE</a>
<br />

<h3>TPL_ARTICLES_EDIT_LIST</h3>

<form action="article_edit.php">TPL_ENTER_ARTICLE_ID<br>
<input type="text" name="id" value="">
<input type="submit" name="TPL_ARTICLE_EDIT" value="Submit"></form>
</Form>
<? // added by blicero for edit all comment to an article ftr ?>
<form action="index.php">TPL_ENTER_COMMENT_ID<br>
<input type="text" name="parent_id_admin" value="">
<input type="hidden" name="comments" value="yes">
<input type="submit" name="TPL_COMMENT_EDIT" value="Submit"></form>
</Form>

TPL_CLICK_ON_ARTICLE_TO_EDIT

TPL_TOP_SEARCH_FORM

TPL_NAV

<FORM method="POST" name="order_form" action="article_bulk_status_change.php">
<INPUT type="submit" name="save" value="TPL_SAVE"/><br >
<br >
TPL_TABLE_MIDDLE
<br />
<INPUT type=submit name="save" value="TPL_SAVE"/>
</FORM>

TPL_NAV

TPL_BOTTOM_SEARCH_FORM
