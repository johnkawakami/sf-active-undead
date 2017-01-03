<?php
if (!$GLOBALS['isWap']) {

// This file really needs to be removed. Right now we have dependencies on it.

$dir = trim(getcwd());
$dir = str_replace(SF_ROOT_URL . '/', '', $dir);

$lvls = array_reverse(split("/",$dir));

$cur_dir = "/";

$cur_crumb = $GLOBALS['site_crumb'];
?>

<div>

<?php

do
{
    if($next_dir)
    {
        $cur_crumb = $next_dir;
        $cur_dir = $cur_dir . $next_dir . "/";
    }

    if (($cur_dir != $_SERVER['REQUEST_URI']) and ($next_dir)) echo " : ";

    if ($cur_dir != $_SERVER['REQUEST_URI']) echo "<a href=\"$cur_dir\">$cur_crumb</a>";

}
while ($next_dir = array_pop($lvls));
?>
 : 
<?php
    echo $Title;
    if (isset($crumb_footer) && $crumb_footer=="check") echo " (<a href=\"#top\">top</a>)";
    $crumb_footer="check"; 
?>
</div>
<?php } // isWap ?>
