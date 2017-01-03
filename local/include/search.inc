<?php if (!isset($medium)) { $medium = null; } ?>
<form method="get" action="/news/"><small><input type="hidden" name="comments" value="yes" />
<select name="medium">
<option value=""><?php echo $sftr->trans('medium'); ?></option>
<option <? if ($medium=="text") echo "selected=\"selected\""; ?> value="text"><?php echo $sftr->trans('text'); ?></option>
<option <? if ($medium=="image") echo "selected=\"selected\""; ?> value="image"><?php echo $sftr->trans('image'); ?></option>
<option <? if ($medium=="audio") echo "selected=\"selected\""; ?> value="audio"><?php echo $sftr->trans('audio'); ?></option>
<option <? if ($medium=="video") echo "selected=\"selected\""; ?> value="video"><?php echo $sftr->trans('video'); ?></option>
<option <? if ($medium=="application") echo "selected=\"selected\""; ?> value="application"><?php echo $sftr->trans('other'); ?></option>
</select><br /><input name="keyword" value="" size="6" /><br />
<input type="submit" value="<?php echo $sftr->trans('search'); ?>" /></small></form>
