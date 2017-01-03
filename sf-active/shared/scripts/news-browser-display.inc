<p><a name="<?=$cell_counter?>" href="<?=$searchlink?>">
<img alt="<?=$mime?> align="middle" border="0" /></a> 
<b><a href="<?=$searchlink?>"><?=$heading?></a></b> 
<small><?=$author?> 
<i><?=$row[date_entered]?></i></small><?
if ($summary) { echo "<br />$summary <a href=\"$searchlink\">(";
if (ereg("audio/mpeg",$row[mime_type])) { echo "MP3"; }
else if (ereg("mp4",$row[mime_type])) { echo "MP4"; }
else if (ereg("real",$row[mime_type])) { echo "realmedia"; }
else if (ereg("quicktime",$row[mime_type])) { echo "quicktime"; }
else if (ereg("audio",$row[mime_type])) { echo $lang_audio; }
else if (ereg("image",$row[mime_type])) { echo $lang_image; }
else if (ereg("video",$row[mime_type])) { echo $lang_video; }
else if (ereg("pdf",$row[mime_type])) { echo "PDF"; }
else { echo $lang_full_story; }
if ($row[numcomment]) echo " $lang_and $row[numcomment] ";
if ($row[numcomment] == 1) echo $lang_comment;
if ($row[numcomment] > 1) echo $lang_comments;
echo ")</a>"; } ?></p>
