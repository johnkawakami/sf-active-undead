<!-- upload add template begins -->

<P><a href="/admin/">TPL_ADMIN_INDEX</a> : TPL_UPLOAD_SUBTITLE</p>

TPL_LOCAL_UPDATE

TPL_LOCAL_UPLIMG

<form action="upload_display_add.php" enctype="multipart/form-data" method="POST">
<br />
TPL_FILE_TO_UPLOAD :
<br />

<input type="file" name="upload_file" />
<input type="hidden" name="MAX_FILE_SIZE" value="1000000" />

<br /><br />
TPL_LOCATION_UPLOAD:
<br />

TPL_DIR_FORM

<br />
<br />
TPL_OVER_WRITE: 
<br />

<input type="checkbox" name="overwrite" value="true" />
<br /><br />

<input type="submit" name="upload" value="upload">
</form>

<!-- 
<h3>TPL_BROWSE_IMAGES</h3>

TPL_DIR_LIST

    upload add template ends -->
