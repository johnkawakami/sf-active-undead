<?php if ($left != "noleft") 
{ 
    ?></td>
    <td width="150" valign="top" style="padding: 3px;"><?php
    if (preg_match('/news/',$_SERVER['REQUEST_URI']))
          {
	       include(SF_CACHE_PATH . '/summaries.html');
               echo "</td></tr></table>"; // close off table from summaries.html
          }
}  
include(SF_INSTALL_PATH . '/local/include/footer.inc');
?>

