<?php 
$GLOBALS['category_id'] = isset($GLOBALS['category_id']) ? $GLOBALS['category_id'] : '';
$feature_name = isset($feature_name) ? $feature_name : '';
if (!$GLOBALS['isMobile']) {
?><!-- center_right.inc -->

</td>
<td width="150" class="bgnewswire" valign="top" style="padding: 3px; border-left: 1px solid black;">

<? // comment the following line out if you don't want minicalendar support
if ( '/'==$_SERVER['REQUEST_URI']) 
{
     include(SF_CACHE_PATH.'/calendar/minical.html');
}
?>
<div align="center">
  <div class="h4">
    <span class="newswirehead"><?php echo $sftr->trans('latest_newswire'); ?></span>
    <a href="http://la.indymedia.org/newswire.rss"><img src="http://la.indymedia.org/im/pod.jpg" border="0" /></a>
  </div>
  <?php include(SF_INCLUDE_PATH . '/media_key.inc'); ?>

  <div align="center" class="h3"><a class="publink" href="/publish.php"><?php echo $sftr->trans('publish_your_news'); ?></a></div>
</div>

<?php
if (strlen($feature_name)>0){
	include(SF_CACHE_PATH . '/'.$feature_name.'_summaries.html'); 
}else{
	include(SF_CACHE_PATH . '/summaries.html');
}
?>

<p><b><a class="publink" href="/news/?page=1&category=<?php echo($GLOBALS['category_id']) ;?>"><?php echo $sftr->trans('latest_news'); ?></a></b></p>
<p><b><a class="publink" href="/uploads/?category_id=<?php echo($GLOBALS['category_id']); ?>"><?php echo($sftr->trans('latest_uploads')); ?></a></b></p>
<p><b><a class="publink" href="/comment_latest.php"><?php echo $sftr->trans('view_latest_comments'); ?></a></b></p>
<p><a class="newswirehed" href="/news/?display=f"><small><?php echo $sftr->trans('view_hidden_posts'); ?></small></a></p>

<!-- end center-right.inc -->
<?php } //!isWap ?>
