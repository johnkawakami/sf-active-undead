<form method="get" action="/news/">
	<input type="hidden" name="comments" value="yes" />
	<select name="medium">
		<option value="">TPL_LANG_MEDIUM</option>
		<option TPL_TEXT_SEL value="text">TPL_LANG_TEXT</option>
		<option TPL_IMAGE_SEL value="image">TPL_LANG_IMAGE</option>
		<option TPL_AUDIO_SEL value="audio">TPL_LANG_AUDIO</option>
		<option TPL_VIDEO_SEL value="video">TPL_LANG_VIDEO</option>
		<option TPL_OTHER_SEL value="application">TPL_LANG_OTHER</option>
	</select>
	<br />
	<input name="keyword" value="" size="6" /><br />
	<input type="submit" value="TPL_LANG_SEARCH" />
</form>
