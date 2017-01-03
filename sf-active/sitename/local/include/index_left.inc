<td width="100" valign="top" class="bgleftcol"><table width="100%" border="0" cellspacing="0" cellpadding="3"><tr><td><div class="left"><small><a class="left" 
href="/features/"><b><?php echo $sftr->trans('features'); ?></b></a><br /><small class="featurelist">
&#8226; <a class="left" href="/news/"><?php echo $sftr->trans('latest_news'); ?></a><br />
&#8226; <a class="left" href="/uploads/"><?php echo $sftr->trans('latest_uploads') ; ?></a><br />
&#8226; <a class="left" href="/syn/"><?php echo $sftr->trans('syndication'); ?></a><br />
&#8226; <a class="left" href="/comment_latest.php"><?php echo $sftr->trans('commentary'); ?></a></small></small></div>

<? include (SF_CACHE_PATH."/left_catlisting.inc"); ?>

<p class="left" align="center"><small><a href="/process/donate.php"><img 
src="/images/pileup.gif" width="95" height="120" border="0" 
alt="donate now" /></a></small></p>

<div class="left"><small><a class="left" href="/process/resources/"><b><?php echo $sftr->trans('resources'); ?></b></a></small></div>

<div class="left"><small><a class="left" href="/process/"><b><?php echo $sftr->trans('participate<'); ?></b></a></small></div>

<div class="left"><small><b><a class="left" href="/archives/"><?php echo $sftr->trans('archives'); ?></a></b></small></div>

<div class="left"><small><b><a class="left" href="/irc/"><?php echo $sftr->trans('chat'); ?></a></b></small></div>

<div class="left"><? include(SF_INSTALL_PATH . '/local/include/search.inc'); ?></div>

<div class="left"><? include(SF_INSTALL_PATH . '/local/include/translate.inc'); ?></div>

<? if ($left) { include("$left"); } ?>

<div class="left"><small><a class="left" href="/process/links.php"><b><?php echo $sftr->trans('links'); ?></b></a></small></div>

<div class="left"><small><a class="left" href="/process/donate.php"><b><?php echo $sftr->trans('donate'); ?></b></a></small></div>

<div class="left"><small><strong><a class="left" href="http://sfactive.indymedia.org/">sf-active</a></strong></small></div>

<div class="left"><small><strong><a class="left" href="/network.php">imc network</a></strong></small></div></td></tr></table></td>
