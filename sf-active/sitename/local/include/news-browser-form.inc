<? include("$local_include_path/category_names.inc"); ?>

<form method="get" name="<? if (!$formname) { $formname="topform"; } 
else { $formname="bottomform"; }
echo $formname; ?>" action="<?=$PHP_SELF?>"
onsubmit="if (document.<?=$formname?>.keyword.value=='<?=$lang_search_by_keyword?>') document.<?=$formname?>.keyword.value='';if (document.<?=$formname?>.author.value=='<?=$lang_search_by_author?>') document.<?=$formname?>.author.value='';">

<input name="keyword" type="text" size="7" 
onFocus="if (document.<?=$formname?>.keyword.value=='<?=$lang_search_by_keyword?>') document.<?=$formname?>.keyword.value='';" 
value="<? if ($keyword) { echo $keyword; } else { echo $lang_search_by_keyword; } ?>" />

<input name="author" type="text" size="6"
onFocus="if (document.<?=$formname?>.author.value=='<?=$lang_search_by_author?>') document.<?=$formname?>.author.value='';" 
value="<? if ($_GET[author]) { echo $_GET[author]; } else { echo $lang_search_by_author; } ?>" />

<input type="checkbox" name="comments" <? if ($comments) echo "checked=\"checked\""; ?> value="yes"><small><?=$lang_comments?></small>

<select name="display"><option value=""><?=$lang_all?></option>
<option <? if ($HTTP_GET_VARS[display]=="l") echo "selected=\"selected\""; ?> value="l"><?=$lang_local?></option>
<option <? if ($HTTP_GET_VARS[display]=="g") echo "selected=\"selected\""; ?> value="g"><?=$lang_global?></option>
<option <? if ($HTTP_GET_VARS[display]=="t") echo "selected=\"selected\""; ?> value="t"><?=$lang_other?></option>
<option <? if ($HTTP_GET_VARS[display]=="f") echo "selected=\"selected\""; ?> value="f"><?=$lang_hidden?></option></select>

<select name="year"><option value=""><?=$lang_year?></option>
<option <? if ($year=="2002") echo "selected=\"selected\""; ?> value="2002">2002</option>
<option <? if ($year=="2001") echo "selected=\"selected\""; ?> value="2001">2001</option>
<option <? if ($year=="2000") echo "selected=\"selected\""; ?> value="2000">2000</option>
</select>

<select name="month"><option value=""><?=$lang_month?></option>
<option <? if ($month=="01") echo "selected=\"selected\""; ?> value="01"><?=$lang_m01?></option>
<option <? if ($month=="02") echo "selected=\"selected\""; ?> value="02"><?=$lang_m02?></option>
<option <? if ($month=="03") echo "selected=\"selected\""; ?> value="03"><?=$lang_m03?></option>
<option <? if ($month=="04") echo "selected=\"selected\""; ?> value="04"><?=$lang_m04?></option>
<option <? if ($month=="05") echo "selected=\"selected\""; ?> value="05"><?=$lang_m05?></option>
<option <? if ($month=="06") echo "selected=\"selected\""; ?> value="06"><?=$lang_m06?></option>
<option <? if ($month=="07") echo "selected=\"selected\""; ?> value="07"><?=$lang_m07?></option>
<option <? if ($month=="08") echo "selected=\"selected\""; ?> value="08"><?=$lang_m08?></option>
<option <? if ($month=="09") echo "selected=\"selected\""; ?> value="09"><?=$lang_m09?></option>
<option <? if ($month=="10") echo "selected=\"selected\""; ?> value="10"><?=$lang_m10?></option>
<option <? if ($month=="11") echo "selected=\"selected\""; ?> value="11"><?=$lang_m11?></option>
<option <? if ($month=="12") echo "selected=\"selected\""; ?> value="12"><?=$lang_m12?></option>
</select>

<select name="day"><option value=""><?=$lang_day?></option>
<option <? if ($day=="0") echo "selected=\"selected\""; ?> value="0">01-09</option>
<option <? if ($day=="1") echo "selected=\"selected\""; ?> value="1">10-19</option>
<option <? if ($day=="2") echo "selected=\"selected\""; ?> value="2">20-29</option>
<option <? if ($day=="3") echo "selected=\"selected\""; ?> value="3">30-31</option>
<option <? if ($day=="01") echo "selected=\"selected\""; ?> value="01">01</option>
<option <? if ($day=="02") echo "selected=\"selected\""; ?> value="02">02</option>
<option <? if ($day=="03") echo "selected=\"selected\""; ?> value="03">03</option>
<option <? if ($day=="04") echo "selected=\"selected\""; ?> value="04">04</option>
<option <? if ($day=="05") echo "selected=\"selected\""; ?> value="05">05</option>
<option <? if ($day=="06") echo "selected=\"selected\""; ?> value="06">06</option>
<option <? if ($day=="07") echo "selected=\"selected\""; ?> value="07">07</option>
<option <? if ($day=="08") echo "selected=\"selected\""; ?> value="08">08</option>
<option <? if ($day=="09") echo "selected=\"selected\""; ?> value="09">09</option>
<option <? if ($day=="10") echo "selected=\"selected\""; ?> value="10">10</option>
<option <? if ($day=="11") echo "selected=\"selected\""; ?> value="11">11</option>
<option <? if ($day=="12") echo "selected=\"selected\""; ?> value="12">12</option>
<option <? if ($day=="13") echo "selected=\"selected\""; ?> value="13">13</option>
<option <? if ($day=="14") echo "selected=\"selected\""; ?> value="14">14</option>
<option <? if ($day=="15") echo "selected=\"selected\""; ?> value="15">15</option>
<option <? if ($day=="16") echo "selected=\"selected\""; ?> value="16">16</option>
<option <? if ($day=="17") echo "selected=\"selected\""; ?> value="17">17</option>
<option <? if ($day=="18") echo "selected=\"selected\""; ?> value="18">18</option>   
<option <? if ($day=="19") echo "selected=\"selected\""; ?> value="19">19</option>
<option <? if ($day=="20") echo "selected=\"selected\""; ?> value="20">20</option>
<option <? if ($day=="21") echo "selected=\"selected\""; ?> value="21">21</option>
<option <? if ($day=="22") echo "selected=\"selected\""; ?> value="22">22</option>
<option <? if ($day=="23") echo "selected=\"selected\""; ?> value="23">23</option>
<option <? if ($day=="24") echo "selected=\"selected\""; ?> value="24">24</option>
<option <? if ($day=="25") echo "selected=\"selected\""; ?> value="25">25</option>
<option <? if ($day=="26") echo "selected=\"selected\""; ?> value="26">26</option>
<option <? if ($day=="27") echo "selected=\"selected\""; ?> value="27">27</option>
<option <? if ($day=="28") echo "selected=\"selected\""; ?> value="28">28</option>   
<option <? if ($day=="29") echo "selected=\"selected\""; ?> value="29">29</option>   
<option <? if ($day=="30") echo "selected=\"selected\""; ?> value="30">30</option>   
<option <? if ($day=="31") echo "selected=\"selected\""; ?> value="31">31</option>   
</select>

<select name="medium"><option value=""><?=$lang_medium?></option>
<option <? if ($medium=="text") echo "selected=\"selected\""; ?> value="text"><?=$lang_text?></option>
<option <? if ($medium=="image") echo "selected=\"selected\""; ?> value="image"><?=$lang_image?></option>
<option <? if ($medium=="audio") echo "selected=\"selected\""; ?> value="audio"><?=$lang_audio?></option>
<option <? if ($medium=="video") echo "selected=\"selected\""; ?> value="video"><?=$lang_video?></option>
<option <? if ($medium=="application") echo "selected=\"selected\""; ?> value="application"><?=$lang_other?></option>
</select>

<? if ($category_selector) { ?>
<select name="category"><option value=""><?=$lang_category?></option>
<option <? if ($category==16) echo "selected=\"selected\""; ?> value="16"><?=$cat16s?></option>
<option <? if ($category==2) echo "selected=\"selected\""; ?> value="2"><?=$cat2s?></option>
<option <? if ($category==15) echo "selected=\"selected\""; ?> value="15"><?=$cat15s?></option>
<option <? if ($category==14) echo "selected=\"selected\""; ?> value="14"><?=$cat14s?></option>
<option <? if ($category==13) echo "selected=\"selected\""; ?> value="13"><?=$cat13s?></option>
<option <? if ($category==17) echo "selected=\"selected\""; ?> value="17"><?=$cat17s?></option>
<option <? if ($category==18) echo "selected=\"selected\""; ?> value="18"><?=$cat18s?></option>
<option <? if ($category==19) echo "selected=\"selected\""; ?> value="19"><?=$cat19s?></option>
<option <? if ($category==20) echo "selected=\"selected\""; ?> value="20"><?=$cat20s?></option>
<option <? if ($category==22) echo "selected=\"selected\""; ?> value="22"><?=$cat22s?></option>
</select><? } else { ?>
<input type="hidden" name="category" value="<?=$category?>" /><? } ?>

<select name="sort">
<option value=""><?=$lang_sort_by?></option>
<option <? if (!$sort) echo "selected=\"selected\""; ?> value=""><?=$lang_newest?></option>
<option <? if ($sort=="reverse") echo "selected=\"selected\""; ?> value="reverse"><?=$lang_oldest?></option>
<option <? if ($sort=="modified") echo "selected=\"selected\""; ?> value="modified"><?=$lang_modified?></option>
<option <? if ($sort=="rank") echo "selected=\"selected\""; ?> value="rank"><?=$lang_comments?></option>
</select>

<select name="limit">
<option <? if ($limit==10) echo "selected=\"selected\""; ?> value="10">10</option>
<option <? if ($limit==20) echo "selected=\"selected\""; ?> value="20">20</option>
<option <? if ($limit==50) echo "selected=\"selected\""; ?> value="50">50</option>
<option <? if ($limit==100) echo "selected=\"selected\""; ?> value="100">100</option>
</select>

<input type="submit" value="<?=$lang_go?>" 
onmouseover="if (document.<?=$formname?>.keyword.value=='<?=$lang_search_by_keyword?>') document.<?=$formname?>.keyword.value='';if (document.<?=$formname?>.author.value=='<?=$lang_search_by_author?>') document.<?=$formname?>.author.value='';" />

</form>
