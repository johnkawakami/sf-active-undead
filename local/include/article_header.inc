<?php
include('johnk.inc');
sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');
?>
<a href="<? echo $GLOBALS['page_id'] ?>.html">printable version</a>
 - <a href="/js/getarticle.php?id=<? echo $GLOBALS['page_id'] ?>">js reader version</a>
 - <a href="/mail.php?id=<? echo $GLOBALS['page_id'] ?>">email this article</a>
 - <a href="/news/hidden.php?id=<? echo $GLOBALS['page_id'] ?>">view hidden posts</a>
 - <a href="/tags/index.php?id=<? echo $GLOBALS['page_id'] ?>">tags and related articles</a>
<br>
<br>
