<?php 

if ($left=="noleft") {
    echo "<table class=\"bgult\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\"> <tr><td><table 
          cellpadding=\"0\" cellspacing=\"1\" border=\"0\">";
} else
{
    echo "</td></tr></table></td></tr></table></td></tr>";
}

?>

<tr>
<td class="bgcities">

<table cellpadding="7" cellspacing="0" border="0" width="100%">
<tr valign="top">
<td><small>

<?php echo "<b>IMC Network:</b> "; sf_include_file(SF_SHARED_PATH, "cities.inc"); ?>

<br /><br />

<?php
include (SF_INSTALL_PATH . '/local/include/disclaimer.inc');
?>
</small></td></tr></table></td></tr></table></td></tr></table>

<? if ($dev_environment==true) { echo "<br /><center><a href=\"http://validator.w3.org/check/referer\">validate html</a> | <a href=\"http://jigsaw.w3.org/css-validator/validator?uri=http://".$HTTP_HOST.$REQUEST_URI."\">validate css</a></center>"; } ?>

</body></html>
