<div>
	<a href="/">TPL_LANG_SITE_CRUMB</a> : 
	<a href="/calendar/">TPL_CAL_LANG_CALENDAR</a> : 
</div>

<br />
	
<div>
	<big>
		<a href="event_display_week.php?day=TPL_CAL_DAY&amp;month=TPL_CAL_MONTH&amp;year=TPL_CAL_YEAR">
		TPL_CAL_LANG_RETURN_TO_CAL
		</a>
	</big> 
	&nbsp; 
	&nbsp; 
    <form action="event_display_edit.php" method="post" style="display: inline">
		<input type="hidden" name="event_id" value="TPL_CAL_EVENT_ID">
        <button>Edit</button>
    </form>
	&nbsp; 
	&nbsp; 
    <form action="event_display_delete.php" method="post" style="display: inline">
		<input type="hidden" name="event_id" value="TPL_CAL_EVENT_ID">
        <button>Delete</button>
    </form>
</div>

TPL_CAL_CACH_FILE
