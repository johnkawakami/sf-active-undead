<?php

$files   = file('wmvfiles.txt');
$deletes = file('wmvdeletes.txt');
$oks     = file('wmvoks.txt');

$action  = $_GET['action'];
$path    = $_GET['path'];

if ($action=='ok')
{
	$fh = fopen('wmvoks.txt','a+');
	fwrite($fh,"$path\n");
	fclose($fh);
	$oks[] = "$path\n";
}
else if ($action=='delete')
{
	$fh = fopen('wmvdeletes.txt','a+');
	fwrite($fh,"$path\n");
	fclose($fh);
	$deletes[] = "$path\n";
}

echo "<table>";
foreach($files as $file)
{
	if (!in_array($file,$oks) && !in_array($file,$deletes))
	{
		$file = rtrim($file);
		$filesize = floor(filesize($file)/(1024*1024)) . "M";
		$f = urlencode($file);
		echo "<tr><td><a href='$file'>$file</a> $filesize</td>";
		echo "<td><a href='?action=ok&path=$f'>ok</a> - 
          <a href='?action=delete&path=$f'>mark for deletion</a></td></tr>";
	}
}
echo "</table>";

?>
