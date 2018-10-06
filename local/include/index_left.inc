<div class="left">
<small><a class="left" 
href="/features/"><b><?php echo $sftr->trans('features'); ?></b></a><br /><small class="featurelist">
• <a class="left" href="/news/"><?php echo $sftr->trans('latest_news'); ?></a><br />
• <a class="left" href="http://la.indymedia.org/news/?keyword=&author=&display=&year=&month=&day=&medium=&language_id=&category=0&sort=rating&limit=30">best of news</a><br />
• <a class="left" href="/syn/"><?php echo $sftr->trans('syndication'); ?></a><br />
• <a class="left" href="/comment_latest.php"><?php echo $sftr->trans('commentary'); ?></a></small></small><br />

<!-- iframe src="http://zanon.slaptech.net/paypal/financeInfo.php?d=544&f=html" width="101" height="252" style="border:none"></iframe -->

<?php include (SF_CACHE_PATH."/left_catlisting.inc"); ?>

<!-- insert allies and weblogs here -->
<p class="left" style="text-align:center"><a href="http://killradio.org">
<img src="/im/meter.gif" border="0"><br />
KILLRADIO</a></p>
<p><a href=http://vozmob.net/>VozMob</a></p>
<p><a href="http://www.abcf.net/losangeles/">ABCF LA</a></p>
<p><a href="http://radio4all.net/">A-Infos Radio</a></p>
<p><a href="http://archive.kpfk.org/index.php?shokey=indymed">Indymedia On Air</a></p>
<p><a href="http://groups.yahoo.com/group/dope_x_resistancela/">Dope-X-Resistance-LA List</a></p>
<p><a href="http://groups.yahoo.com/group/laamn/messages">LAAMN List</a></p>


<!-- p class="left" align="center"><small><a href="/process/donate.php"><img 
src="/images/pileup.gif" width="95" height="120" border="0" 
alt="donate now" /></a></small></p -->

<div class="left"><small><a class="left" href="/process/resources/"><b><?php echo $sftr->trans('resources'); ?></b></a></small></div>

<div class="left"><small><a class="left" href="/process/"><b><?php echo $sftr->trans('participate<'); ?></b></a></small></div>

<div class="left"><small><b><a class="left" href="/archives/"><?php echo $sftr->trans('archives'); ?></a></b></small></div>

<div class="left"><?php include(SF_INSTALL_PATH . '/local/include/search.inc'); ?></div>

<div class="left"><small><a class="left" href="/process/links.php"><b><?php echo $sftr->trans('links'); ?></b></a></small></div>

<div class="left"><small><a class="left" href="/process/donate.php"><b><?php echo $sftr->trans('donate'); ?></b></a></small></div>

<div class="left"><small><strong><a class="left" href="http://sfactive.indymedia.org/">sf-active</a></strong></small></div>

<div class="left"><small><strong><a class="left" href="/network.php">imc network</a></strong></small></div>
<br />
<div class="left"><small><strong><?php echo "<b>IMC Network:</b></strong> "; sf_include_file(SF_SHARED_PATH, "cities.inc"); ?></small></div>

