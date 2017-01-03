<div<?php
if (!is_int($cell_counter/2)) {
	?> class="bgsearchgrey"<?
} ?>><a name="<?=$cell_counter?>" href="<?=$searchlink?>"><img alt="<?=$mime?> border="0" align="middle" /></a> 
<tt><?=$row[id]?>[<?=$row[parent_id]?>]</tt> <?php
if ($row[display]=="f") echo "<strike>";
echo "<small><i>$row[date_entered]</i></small> ";
echo "<b><a href=\"$searchlink\">$heading</a></b>";
echo " $author ";
echo " <small>($row[numcomment] ";
if ($row[numcomment] == 1) { echo $lang_comment; } else { echo $lang_comments; }
echo ")</small>";
if ($row[display]=="f") echo "</strike>";
echo " <a href=\"article_edit.php?id=";
echo $row[id];
echo "\">$lang_edit</a> ";
?><input type="hidden" name="id_<?=$rownum?>" value="<?=$row[id]?>"/>
<input type="hidden" name="old_display_<?=$rownum?>" value="<?=$row[display]?>"/>
<select name="value_<?=$rownum?>">
<option value="t" <? if ($row[display]=="t") { ?>selected="selected"<? } ?>><?=$lang_display?></option>
<option value="f" <? if ($row[display]=="f") { ?>selected="selected"<? } ?>><?=$lang_hidden?></option>
<option value="l" <? if ($row[display]=="l") { ?>selected="selected"<? } ?>><?=$lang_local?></option>
<option value="g" <? if ($row[display]=="g") { ?>selected="selected"<? } ?>><?=$lang_global?></option>
</select>

<?
/* cycles through all the categories and print out the belonging categories at the end of the article admin line */
$catq="select catlink.catid,category.shortname from catlink left join category on catlink.catid = category.category_id where id=".$row[id];
$catresult = $db_obj->query($catq);
if (!empty($catresult)) { echo "article categories:  ";}
while ($catarray=array_pop($catresult)) {
echo $catarray[1]."  ";
}
?>


</div><?
$rownum=$rownum+1;
?>
