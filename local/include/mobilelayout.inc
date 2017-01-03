<?php
echo  '<?xml version="1.0"?>';
?><!DOCTYPE html PUBLIC "-//WAPFORUM//DTD XHTML Mobile 1.2//EN" "http://www.openmobilealliance.org/tech/DTD/xhtml-mobile12.dtd">
<html>
<head>
<style type="text/css">
.box { 
   padding: 0.5em;
   padding-right: 3em; 
   border: 1px solid gray; 
   border-color: white black black white; 
   background-color: #ddd; 
}
h2 { 
   padding: 0.5em;
   background-color: black; 
   color: white; 
   margin-top: 2em;
   opacity: 0.7;
}
body { background-color: #bbb; 
background-image: url(http://la.indymedia.org/uploads/2006/03/click_picture_for_full_resolution_93.jpgmid.jpg);
}
a:link { color: black; }

</style>
</head>
<body>
<h1 style="color:darkred; text-shadow: #000 2px 2px 2px;">LA.Indymedia.org</h1>
<p style="color:white; background-color: black; opacity:0.7;">This is an experimental layout.  To view the original page 
<a href="/?mobile=0" style="color:red">click here</a>.
</p>
<p style="color:white; background-color: red; opacity:0.8;"><a href="/js/" style="color:white; font-weight: bold">Tap to try the new smartphone reader.</a> currently in development.</p>
<h2>Calendar</h2>
<?php 
    include(SF_SHARED_PATH . '/classes/calendar/minical.inc');
    $m = new Minical();
    $events = $m->event_array;
    foreach($events as $event) {
           ?><div class="box"><?=$event->start_month.'/'.$event->start_day?> <a href="/calendar/event_display_detail.php?event_id=<?=$event->id?>"><?=$event->title?></a></div><?  
    } 
?>
<div class="box"><b><a href="http://la.indymedia.org/calendar/event_display_list.php">Full Calendar</a></b></div>
<h2>Breaking Newswire (Unfiltered)</h2>
<?php 
    include_once(SF_SHARED_PATH . '/classes/category_class.inc');
    include(SF_SHARED_PATH . '/classes/feature_class.inc');
    include(SF_SHARED_PATH . '/classes/newswire_class.inc');
    $n = new Newswire();
    $articles = $n->get_summary_list(15,'t',null);
    for($i = count($articles); $i-- ; ) {
        ?><div class="box"><a href="/js/getarticle.php?id=<?=$articles[$i]['id']?>"><?=$articles[$i]['heading']?></a></div><?php
    }
?>
<h2>Features (Editor-Written)</h2>
<?php
    $feature_obj= new Feature;
    $features = $feature_obj->get_list(3,'c');
    $features = array_reverse($features);
    foreach($features as $feature) {
        ?><div class="box"><a href="/archives/archive_by_id.php?id=<?=$feature['feature_id']?>&category_id=3"><?=$feature['title1']?></a></div><?php
    }
?>
<h2>Selected Local News</h2>
<?php 
    $n = new Newswire();
    $articles = $n->get_summary_list(15,'l',null);
    for($i = count($articles); $i-- ; ) {
        ?><div class="box"><a href="/print.php?id=<?=$articles[$i]['id']?>"><?=$articles[$i]['heading']?></a></div><?php
    }
?>
<p style="background-color: white;">Photo by Marcus</p>
<p style="background-color: white;">LA Indymedia is an Open Publishing system.  To post stories, go to <a href="publish.php">the publish page</a>.  You should probably do this from a regular computer.</p>
</body></html>
