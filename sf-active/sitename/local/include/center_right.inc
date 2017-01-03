</td></tr></table>
</td></tr></table>
</td>

<td class="bgult" width="1"><img src="/im/bump.gif" width="1" height="1" alt="" /></td>

<td width="150" class="bgnewswire" valign="top">
<table width="100%" border="0" cellspacing="0"  cellpadding="3">
<tr><td valign="top">

<? // comment the following line out if you don't want minicalendar support
include(SF_CACHE_PATH.'/calendar/minical.html');
?>
<div align="center">
<div class="h4">
<span class="newswirehead"><?php echo $sftr->trans('latest_newswire'); ?></span>
</div>

<?php

include(SF_INCLUDE_PATH . '/media_key.inc');

?>

<div align="center" class="h3"><a class="publink" href="/publish.php"><?php echo $sftr->trans('publish_your_news'); ?></a></div></div>

<?php
if (strlen($feature_name)>0){
	include(SF_CACHE_PATH . '/'.$feature_name.'_summaries.html'); 
}else{
	include(SF_CACHE_PATH . '/summaries.html');
}
?>
<p><b><a class="publink" href="/news/?page=1&amp;category=<?php echo($GLOBALS['category_id']) ;?>"><?php echo $sftr->trans('latest_news'); ?></a></b></p>
<p><b><a class="publink" href="/uploads/?category_id=<?php echo($GLOBALS['category_id']); ?>"><?php echo($sftr->trans('latest_uploads')); ?></a></b></p>
<p><b><a class="publink" href="/comment_latest.php"><?php echo $sftr->trans('view_latest_comments'); ?></a></b></p>
<p><a class="newswirehed" href="/news/?display=f"><small><?php echo $sftr->trans('view_hidden_posts'); ?></small></a></p>


