<?php
$Title = "Refresh";
include("shared/global.cfg");
$db_obj = new DB();
include("$local_include_path/content-header.".$language."inc");

$tb_name="webcast";
$seq_name="webcastid";

if (!( $id || ($start && $stop) ) ) {
  echo "missing parameters";
}
else {
  if ($start && ($start<=$stop)) {
    for ($i=$start; $i<=$stop; $i++) {
      $article_id = $id;
      $id = $i;
      include("shared/scripts/cache_page.inc");
      echo $publish_result;
      $publish_result = '';
      $headerstring = '';
    }
  }
  else {
    $article_id = $id;
    include("shared/scripts/cache_page.inc");
    echo $publish_result;
  }
}

include("$local_include_path/content-footer.inc"); ?>
