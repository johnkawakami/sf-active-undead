<?php

// Class for logedit_index page

class logedit_index extends Page {

    function logedit_viewer() {
        return 1;
    }
    
    function execute() {
	// first i get all css
	$dir = opendir(SF_WEB_PATH."/themes/");
	while($item = readdir($dir)){
	if (eregi ( "(.)+\\.css$", $item)) $htmllistcss.="<a href=\"css_viewer.php?filename=".$item."\">".$item."</a><br>"; 
	}	
	$this->tkeys[css_list]=$htmllistcss;

	$dir = opendir(SF_TEMPLATE_PATH);
	while($item = readdir($dir)){
	if (eregi ( "(.)+\\.tpl$", $item)) $htmllisttpl.="<a href=\"template_viewer.php?filename=".$item."\">".$item."</a><br>"; 
	}	
	$this->tkeys[template_list]=$htmllisttpl;
	
	$dir = opendir(SF_INCLUDE_PATH);
	while($item = readdir($dir)){
	if (eregi ( "(.)+\\.(inc$|php$)", $item)) $htmllist.="<a href=\"include_viewer.php?filename=".$item."\">".$item."</a><br>";
	}
	$this->tkeys[include_list]=$htmllist;

	$dir = opendir(SF_CACHE_PATH."/pages/");
	while($item = readdir($dir)){
	if (eregi ( "(.)+\\.(inc$|php$)", $item)) $htmllistpage.="<a href=\"page_viewer.php?filename=".$item."\">".$item."</a><br>";
	}
	$this->tkeys[page_list]=$htmllistpage;
    }
}

?>