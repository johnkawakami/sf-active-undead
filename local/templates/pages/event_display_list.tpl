<form action="event_display_list.php" method="post">
	<table>
		<tr>
			<td>
				TPL_CAL_LANG_TOPIC:
			</td>
			<td>
					TPL_CAL_DD_EVENT_TOPIC
			</td>
		</tr>
		<tr>
			<td>
				TPL_CAL_LANG_TYPE:
			</td>
			<td>
					TPL_CAL_DD_EVENT_TYPE
			</td>
		</tr>
		<tr>
			<td>
				TPL_CAL_LANG_LOCATION:
			</td>
			<td>
					TPL_CAL_DD_LOCATION
			</td>
		</tr>
		<tr>
			<td>
				TPL_CAL_LANG_START_DATE:
			</td>
			<td>
				<select name="begin_month">
					TPL_CAL_DD_START_MONTH
				</select>
				&nbsp;/&nbsp;
				<select name="begin_day">
					TPL_CAL_DD_START_DAY
				</select>
				<select name="begin_year">
					TPL_CAL_DD_START_YEAR
				</select>
			</td>
		</tr>
		<tr>
			<td>
				TPL_CAL_LANG_END_DATE:
			</td>
			<td>
				<select name="end_month">
					TPL_CAL_DD_MONTH_END
				</select>
				&nbsp;/&nbsp;
				<select name="end_day">
					TPL_CAL_DD_DAY_END
				</select>
				<select name="end_year">
					TPL_CAL_DD_YEAR_END
				</select>
			</td>
		</tr>
		<tr>
			<td>
				TPL_CAL_LANG_DISPLAY_DETAILS:
			</td>
			<td>
				<input type="checkbox" name="detail_flag" value="TPL_CAL_DF" TPL_CAL_DF_SELECT />
			</td>
		</tr>
		<tr>
			<td>
				<input type="submit" name="Filter" value="TPL_CAL_LANG_VIEW_EVENTS" />
			</td>
			<td>
				&nbsp;
			</td>
		</tr>
	</table>
</form>

TPL_CAL_EVENT_LIST

<br /><br />
