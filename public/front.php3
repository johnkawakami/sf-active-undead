<?
if (isset($_GET['article_id']))
{
	header("Location: /display.php?id=".$_GET['article_id']);
}
else
{
	header("Location: /news/index.php");
}
?>
