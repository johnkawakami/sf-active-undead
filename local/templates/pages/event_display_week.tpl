<table cellpadding="7" cellspacing="0" border="0" width="100%">
	<tr>
		<td colspan="3">
			TPL_FORM_SEARCH_TOP
		</td>
	</tr>
	<tr>
		<td>
			TPL_CAL_EVENT_MONTH_VIEW_PREV
		</td>
		<td align="center" valign="top">
			<br />
			<p class="eventNav">
				<a href="/">TPL_CAL_LANG_SITE_CRUMB</a> : <a href="/calendar/">TPL_CAL_LANG_CALENDAR</a>
			</p>			
			<p class="eventNav">
				<a 
href="event_display_week.php?day=TPL_CAL_LAST_WD&amp;month=TPL_CAL_LAST_WM&amp;year=TPL_CAL_LAST_WY&amp;topic_id=TPL_CAL_ID_TOPIC&amp;location_id=TPL_CAL_ID_LOCATION">
					TPL_CAL_LANG_PREV_WEEK
				</a> 
				&nbsp; 
				TPL_CAL_LANG_WEEK_OFF 
				TPL_CAL_CUR_DATE 
				&nbsp;
				<a 
href="event_display_week.php?day=TPL_CAL_NEXT_WD&amp;month=TPL_CAL_NEXT_WM&amp;year=TPL_CAL_NEXT_WY&amp;topic_id=TPL_CAL_ID_TOPIC&amp;location_id=TPL_CAL_ID_LOCATION">
					TPL_CAL_LANG_NEXT_WEEK
				</a>
			</p>
			<p class="eventNav2">
				<a href="event_display_add.php">
					TPL_CAL_LANG_ADD_AN_EVENT
				</a>
				&nbsp;
				&nbsp;
				<a href="event_display_monthview.php">
					TPL_CAL_LANG_VIEW_ALL_MONTHS
				</a>
				&nbsp;
				&nbsp;
				<a href="event_display_list.php">				
					TPL_CAL_LANG_LIST_FUTURE_EVENTS
				</a>
			</p>			
				<form action="event_display_week.php" method="get">
					TPL_CAL_LANG_FILTER_BY_TOPIC
						TPL_CAL_EVENT_TOPIC_DROPDOWN
					TPL_CAL_LANG_FILTER_BY_LOCATION
						TPL_CAL_EVENT_LOCATION_DROPDOWN
						<input type="hidden" name="day" value="TPL_CAL_DAY" />
					<input type="hidden" name="month" value="TPL_CAL_MONTH" />
					<input type="hidden" name="year" value="TPL_CAL_YEAR" />

					<input type="submit" name="Filter" value="TPL_CAL_LANG_FILTER"/>
				</form>
		</td>
		<td align="right">
			TPL_CAL_EVENT_MONTH_VIEW_NEXT
		</td>
	</tr>
</table>

<table width="100%" cellspacing="1" cellpadding="5" class="bodyClass">
	<tr>
		TPL_CAL_EVENT_MONTH_DAYTITLE
	</tr>
	TPL_CAL_EVENT_MONTH_VIEW_FULL
</table>
   <br />TPL_FORM_SEARCH_BOTTOM


