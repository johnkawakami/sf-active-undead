<?  $sftr->create_translate_table('event_display_add'); 
    $sftr->create_translate_table('event_display_week');
?>
<div align="center" class="h4"><span class="newswirehead"><? echo($sftr->trans('cal_list_future_events')); ?></span></div>
EVENTS
<p><a class="publink" href="<? echo(SF_CALENDAR_URL);?>"><? echo($sftr->trans('calendar'));?></a> <br />
    <b><a class="publink" href="<? echo(SF_CALENDAR_URL);?>/event_display_add.php"><? echo($sftr->trans('cal_add_an_event'));?></a></b></p>
