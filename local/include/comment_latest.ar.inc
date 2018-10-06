<p>This is a list of the latest comments posted on the site. It is here so that you can more easily
participate in commenting on articles that are posted, and continuing discussions that have started. See also
the <a href="/news/?sort=rank">most commented articles</a>.</p>

<table width="100%" border="0" cellspacing="2" cellpadding="2">
  <tr>
    <td><b><small>ARTICLE:</small></b></td>
    <td><b><small>LATEST AUTHOR:</small></b></td>
    <td><b><small>POSTED AT:</small></b></td>
  </tr>

<?php

include(SF_CACHE_PATH . '/comments_latest.inc');

?>

</table>

<p align="right"><a href="/news/?page=5&amp;sort=modified">Browse older threads&gt;&gt;</a></p>

<p><small>Last updated <?php echo date("r",filemtime("$local_cache_path/comments_latest.inc")); ?>.</small></p>
