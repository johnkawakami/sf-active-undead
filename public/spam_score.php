<?php
include_once("shared/global.cfg");
include(SF_CLASS_PATH."/spamc_class.inc");
# include(SF_CLASS_PATH."/article_class.inc");

$id = $_GET['id'];
if ($id)
{
  if (!preg_match('/^[0-9]{1,7}$/', $id)) die();
  $article = new Article($id);
  $text = $article->article['summary'] . '  ';
  $text .= $article->article['article'];
  $s = new SpamC(strip_tags($text));
}
else
{
	echo "<a href='http://la.indymedia.org/'>IMC</a>";
	exit;
}
function print_hash($hash) 
{
  echo "<table>";
  foreach($hash as $k=>$v)
    echo "<tr><td>$k</td><td>$v</td></tr>";
  echo "</table>";
}
?>
<h1>Spam Score: <?=$s->spam_score();?></h1>
<p>Word count: <?=$s->word_count?></p>
<h2>Keywords</h2>
<p><? print_hash($s->keywords); ?></p>
<h2>Keyword Density</h2>
<p>Word Count = <?=$s->word_count;?></p>
<? print_hash($s->keyword_density); ?>
<h2>Top Keyword Share</h2>
<? print_hash($s->keyword_shares) ?>
<h2>Matches with spam_strings.txt</h2>
<? print_hash($s->matches) ?>
