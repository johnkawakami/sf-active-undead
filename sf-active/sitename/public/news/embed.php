<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<head><title><?php
$title=stripslashes($_GET["title"]); echo $title; 
?></title></head><body><center>
<object classid="clsid:02BF25D5-8C17-4B23-BC80-D3488ABDDC6B" width="100%"
height="100%" codebase="http://www.apple.com/qtactivex/qtplugin.cab">
<param name="SRC" value="<?=$_GET["src"]?>">
<param name="AUTOPLAY" value="true">
<param name="CONTROLLER" value="true">
<embed src="<?=$_GET["src"]?>" controller="true" autoplay="true"
cache="true" targetcache="true" width="100%" height="100%" 
pluginspage="http://www.apple.com/quicktime/download/">
</embed></object></center></body>
