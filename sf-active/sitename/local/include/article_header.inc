<?php

sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');

?>

<table width="100%" cellspacing="0" cellpadding="0" border="0">
<tr><td><a href="/print.php?id=<? echo $GLOBALS['page_id'] ?>">printable version</a>
 - <a href="/mail.php?id=<? echo $GLOBALS['page_id'] ?>">email this article</a></td>
<td align="right">
<?php

echo $GLOBALS["category_name"];
?>
</td></tr></table>
