<!-- TEMPLATE -->
<?php
	include_once( SF_CLASS_PATH . '/bot_class.inc' );
	$bot = new Bot();
?>
<div class= "hidden">
	<code><big>The following post has status <i>hidden</i>:</big></code>
	<?php if ($bot->isHuman()): ?>
			<p class="heading"><a name="C_ANCHOR"><strong class="heading">H_EADING</strong></a><br />
			by A_UTHOR <a href="/news/?author=U_RLAUTH&comments=yes">&#8226;</a> 
			<em>C_DATE</em><br />
			<a href="mailto:C_ONTACT">C_ONTACT</a> V_ALIDATED P_HONE A_DDRESS</p>
			<blockquote class="summary">S_UMMARY</blockquote>
			<p class="media">M_EDIA</p>
			<p class="article">A_RTICLE</p>
			<p class="link"><a href="L_INK">C_ROPURL</a></p>
		    <form action="/comment.php" method="get" class="addcomment">
		     <button>add your comments</button>
		     <input type="hidden" name="top_id" value="A_RTID" />
		    </form><br />

	<?php endif; ?>
</div><br />
<!-- /TEMPLATE -->
