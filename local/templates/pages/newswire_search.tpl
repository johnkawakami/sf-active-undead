<form method="get" name="TPL_FORM_NAME" 
onsubmit="if (document.TPL_FORM_NAME.keyword.value=='TPL_KEYWORD') document.TPL_FORM_NAME.keyword.value='';if (document.TPL_FORM_NAME.author.value=='TPL_AUTHOR') document.TPL_FORM_NAME.author.value='';">

<input name="keyword" type="text" size="7" 
onFocus="if (document.TPL_FORM_NAME.keyword.value=='TPL_KEYWORD') document.TPL_FORM_NAME.keyword.value='';" 
value="TPL_SEARCH_KEYWORD" />

<input name="author" type="text" size="6"
onFocus="if (document.TPL_FORM_NAME.author.value=='TPL_AUTHOR')document.TPL_FORM_NAME.author.value='';" 
value="TPL_SEARCH_AUTHOR" />

<input type="checkbox" name="comments" value="yes" TPL_SEARCH_COMMENTS /><small>TPL_COMMENTS</small>

TPL_SEARCH_DISPLAY

TPL_SEARCH_YEAR

TPL_SEARCH_MONTH

TPL_SEARCH_DAY

TPL_SEARCH_MEDIUM

TPL_SEARCH_LANGUAGE

TPL_SEARCH_CATEGORY

TPL_SEARCH_SORT

TPL_SEARCH_LIMIT

<input type="checkbox" name="summaries" value="no" TPL_SEARCH_SUMMARIES /><small>TPL_NO_SUMMARIES</small>

<input type="submit" value="TPL_GO" 
onmouseover="if (document.TPL_FORM_NAME.keyword.value=='TPL_KEYWORD') document.TPL_FORM_NAME.keyword.value='';if (document.TPL_FORM_NAME.author.value=='TPL_AUTHOR') document.TPL_FORM_NAME.author.value='';" />

</form>
