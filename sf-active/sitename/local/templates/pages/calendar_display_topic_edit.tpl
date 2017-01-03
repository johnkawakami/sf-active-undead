<a href="/admin/calendar/">TPL_ADMIN_INDEX</a> - <a href="/admin/calendar/display_location_list.php">TPL_LOCATION_MENU</a> - <a href="/admin/calendar/display_type_list.php">TPL_TYPE_MENU</a> - <a href="/admin/calendar/display_topic_list.php">TPL_TOPIC_MENU</a> - <a href="event_refresh_all.php">TPL_REGEN_EVENT</a> - <a href="event_display_lookup.php">TPL_LOOKUP_CONF</a><br /><br />

<h3>TPL_SUBTITLE</h3>

<form name ="calendar_display_topic_edit" method="post" action="TPL_LOCAL_FORM">

<table>
    <tr>
    	<td colspan="2">
	  <input type=submit name="save2" value="TPL_SAVE" /><br /><br />
	</td>
    </tr>
	<tr>
		<td colspan="2">
			<input type="hidden" name="id" value="TPL_LOCAL_ID" />TPL_LOCAL_ID
		</td>
	</tr>
	<tr>
		<td>TPL_EVENT_TOPIC_NAME</td>
		<td><input type="text" size="40" maxsize="50" name="name" value="TPL_LOCAL_NAME" /><br /><br /></td>
	</tr>
    <tr>
    	<td colspan="2">
	  <input type=submit name="save2" value="TPL_SAVE" />
	</td>
    </tr>
</table>
