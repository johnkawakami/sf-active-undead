<?php
echo  '<?xml version="1.0"?>';
?><!DOCTYPE wml PUBLIC "-//WAPFORUM//DTD WML 1.1//EN" "http://www.wapforum.org/DTD/wml_1.1.xml"><wml>
<template>
  <do name="a1" type="accept" label="Features">
    <go href="#features"/>
  </do>
  <do name="a2" type="accept" label="Local News">
    <go href="#localnewswire"/>
  </do>
  <do name="a2" type="accept" label="Breaking News">
    <go href="#newswire"/>
  </do>
  <do name="a3" type="accept" label="Calendar">
    <go href="#calendar"/>
  </do>
</template>
<card id="index" title="LA Indymedia" newcontext="true">
<b>LA Indymedia</b><br />
<small>Not even "beta" - it's "alpha."</small><br />
</card>
<card id="features" title="IMC Features"><small>
<?php
    include_once(SF_SHARED_PATH . '/classes/category_class.inc');
    include(SF_SHARED_PATH . '/classes/feature_class.inc');
    $feature_obj= new Feature;
    $features = $feature_obj->get_list(3,'c');
    $features = array_reverse($features);
    foreach($features as $feature) {
        ?>* <a href="/archives/archive_by_id.php?id=<?=$feature['feature_id']?>"><?=$feature['title1']?></a><br /><?php
    }
?></small>
</card>
<card id="localnewswire" title="IMC Local News"><small>
<?php 
    include(SF_SHARED_PATH . '/classes/newswire_class.inc');
    $n = new Newswire();
    $articles = $n->get_summary_list(15,'l',null);
    for($i = count($articles); $i-- ; ) {
        ?>* <a href="/print.php?id=<?=$articles[$i]['id']?>"><?=$articles[$i]['heading']?></a><br /><?php
    }
?></small>
</card>
<card id="newswire" title="IMC Newswire"><small>
<?php 
    $n = new Newswire();
    $articles = $n->get_summary_list(15,'t',null);
    for($i = count($articles); $i-- ; ) {
        ?>* <a href="/print.php?id=<?=$articles[$i]['id']?>"><?=$articles[$i]['heading']?></a><br /><?php
    }
?></small>
</card>
<card id="calendar" title="IMC Calendar"><small>
<?php 
    include(SF_SHARED_PATH . '/classes/calendar/minical.inc');
    $m = new Minical();
    $events = $m->event_array;
    foreach($events as $event) {
           ?><?=$event->start_month.'/'.$event->start_day?> <a href="/calendar/event_display_detail.php?event_id=<?=$event->id?>"><?=$event->title?></a><br /><?php  
    } 
?></small></card>
</wml>
