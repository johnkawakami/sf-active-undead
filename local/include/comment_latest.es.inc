<p>Aqui aparece una lista de los &#250;ltimos comentarios publicados en el sitio. 
Esta aqui para que tu puedas m&#225;s facilmente participar comentando los articulos, y continuar las discusiones que recien comenzaron.</p>

<table width="100%" border="0" cellspacing="2" cellpadding="2">
  <tr>
    <td><b><small>ARTICULO:</small></b></td>
    <td><b><small>ULTIMO AUTOR:</small></b></td>
    <td><b><small>FECHA PUBLICAD:</small></b></td>
  </tr>

<?php

include(SF_CACHE_PATH . '/comments_latest.inc');

?>

</table>

<p><small>Ultima actualizaci&#243;n <?php echo date("r",filemtime("$local_cache_path/comments_latest.inc")); ?>.</small></p>

