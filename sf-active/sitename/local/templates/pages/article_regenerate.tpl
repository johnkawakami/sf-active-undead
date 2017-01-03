<form method="post" action="/admin/article/article_regenerate.php">
<a href="../">TPL_ADMIN_INDEX</a> :  
<a href="./">TPL_ADMIN_ARTICLE</a> : TPL_REGEN_ARTICLE

TPL_LOCAL_RESULT_HTML

<table>

<tr>
<td>TPL_START_ID:</td>
<td><input name="start_id" value="TPL_VALUE_START_ID"></td>
</tr>

<tr>
<td>TPL_MAX_TO_GEN:</td>
<td><input name="max_to_gen" value="TPL_VALUE_MAX_TO_GEN"></td>
</tr>

</table>

<input type="hidden" name="regenerate" value="true"><br>
<input type="submit" name="Regenerate" value="TPL_REGENERATE" />
</form>
