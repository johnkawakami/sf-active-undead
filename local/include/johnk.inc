<?php

function thisPageUrlSelect()
{
	echo "<form style='margin:0px;padding:0px'>link: <input type='text'";
	echo " style='font-size:10px; border-style:solid; width:400px;' ";
	echo "value='";
    echo preg_replace("/'/","&apos;","<a href=\"http://{$_SERVER['SERVER_NAME']}{$_SERVER['REQUEST_URI']}\">".$GLOBALS['page_title']."</a>");
	echo "' "; 
	echo " onclick='this.focus();this.select()' /></form>";
}
?>
