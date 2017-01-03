        <div class="error">
            TPL_CAL_ERROR_MSG
        </div>
        <div>
            <a href="event_display_week.php">TPL_CAL_LANG_RETURN_TO_CALENDAR</a>
        </div>
        <form enctype="multipart/form-data" name="TPL_CAL_FORM_NAME" action="TPL_CAL_FORM_FILE" method="post">            
			<input type="submit" name="add" value="TPL_CAL_ACTION_TITLE" /> 
			<input type="hidden" name="event_id" value="TPL_CAL_EVENT_ID" /> 
			<input type="hidden" name="caction" value="TPL_CAL_CACTION" /> 
            <table>
                <tr>
                    <td>
                        TPL_CAL_LANG_TITLE
                    </td>
                    <td>
                        <input name="title" value="TPL_CAL_PTITLE" size="40" /> 
                    </td>
                </tr>
		<tr>
		    <td>TPL_LANGUAGE
		    </td>
		    <td>
			TPL_LOCAL_LANGUAGE
		    </td>
		</tr>
                <tr>
                    <td>
                        TPL_CAL_LANG_START_DATE
                    </td>
                    <td>
                        <select name="start_month">
							TPL_CAL_DD_START_MONTH
                        </select> 							
						<select name="start_day">
							TPL_CAL_DD_START_DAY
                        </select> 
						<select name="start_year">
							TPL_CAL_DD_START_YEAR
                        </select> 
                    </td>
                </tr>
                <tr>
                    <td>
                        TPL_CAL_LANG_START_TIME
                    </td>
                    <td>
                        <select name="start_hour">
							TPL_CAL_DD_START_HOUR
                        </select>
						<select name="start_minute">
							TPL_CAL_DD_START_MINUTE
                        </select>
						<select name="am">
							TPL_CAL_DD_AMPM
                        </select> 
                    </td>
                </tr>
                <tr>
                    <td>
                        TPL_CAL_LANG_DURATION
                    </td>
                    <td>
                        <input name="duration" size="2" value="TPL_CAL_PDURATION" /> hours
                    </td>
                </tr>
                <tr>
                    <td>
                        TPL_CAL_LANG_LLOCATION
                    </td>
                    <td>
							TPL_CAL_DD_LOCATION_ID
						&nbsp; 
						TPL_CAL_LANG_OTHER 
						<input name="location_other" value="TPL_CAL_LOCATION_OTHER" /> 
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        TPL_CAL_LANG_LOCATION_DETAILS
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <textarea name="location_details" rows="4" cols="40">TPL_CAL_LOCATION_DETAILS</textarea> 
                    </td>
                </tr>
                <tr>
                    <td>
                        TPL_CAL_LANG_EVENT_TYPE
                    </td>
                    <td>
							TPL_CAL_DD_EVENT_TYPE_ID
						&nbsp; TPL_CAL_LANG_OTHER <input name="event_type_other" value="TPL_CAL_EVENT_TYPE_OTHER" /> 
                    </td>
                </tr>
                <tr>
                    <td>
                        TPL_CAL_LANG_EVENT_TOPIC
                    </td>
                    <td>
							TPL_CAL_DD_EVENT_TOPIC_ID
						&nbsp; TPL_CAL_LANG_OTHER <input name="event_topic_other" value="TPL_CAL_EVENT_TOPIC_OTHER" /> 
                    </td>
                </tr>
                <tr>
                    <td>
                        TPL_CAL_LANG_CONTACT_NAME
                    </td>
                    <td>
                        <input name="contact_name" value="TPL_CAL_CONTACT_NAME" /> 
                    </td>
                </tr>
                <tr>
                    <td>
                        TPL_CAL_LANG_CONTACT_EMAIL
                    </td>
                    <td>
                        <input name="contact_email" value="TPL_CAL_CONTACT_EMAIL" /> 
                    </td>
                </tr>
                <tr>
                    <td>
                        TPL_CAL_LANG_CONTACT_PHONE
                    </td>
                    <td>
                        <input name="contact_phone" value="TPL_CAL_CONTACT_PHONE" /> 
                    </td>
                </tr>
		<tr>
		    <table width="100%" border="0" cellspacing="2" cellpadding="4">
			<tr class="bgpenult" valign="top">

			    <td width="25%">
				TPL_ATTACH_A_FILE<br />
				<input type="hidden" name="linked_file_title_1" value="test" />
				<input type="file" name="linked_file_1" /><br />
				TPL_MAX_UPLOAD TPL_UPLOAD_MAX_FILESIZE; TPL_TOTAL_LIMIT TPL_POST_MAX_SIZE; TPL_MAX_EXECUTION_TIME TPL_MINUTES
				<br />TPL_ACCEPTED_TYPES
			    </td>

		    </tr></table>
		</tr>
                <tr>
                    <td colspan="2">
                        TPL_CAL_LANG_DESCRIPTION
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <textarea name="description" cols="50" rows="30">TPL_CAL_DISCRIPTION</textarea> 
                    </td>
                </tr>
		<tr>
		    <td colspan="2">
			TPL_PUB_ART<br /><br />
			<b>TPL_PUB_ARTMIME</b><br />
			TPL_SELECT_ARTMIME<br />
			TPL_PUB_MIME</td>
		</tr>
            </table>
            <input type="submit" name="add" value="TPL_CAL_ACTION_TITLE" />
        </form>
