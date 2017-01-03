<!-- Translate Form Template -->
<form action="http://fets3.freetranslation.com:5081/" method="post">
	<small>
	<a class="left" href="/translate.php"><b><?=$lang_translate?></b></a>
	<input name="Sequence" value="core" type="hidden" />
	<input name="Url" value="<? echo $HTTP_HOST ?><? echo $REQUEST_URI ?>" type="hidden" />
	<select name="Language">
		<option value="English/German">deutsch</option>
		<option value="German/English">de &raquo; en</option>
		<option selected="selected" value="English/Spanish">espa&ntilde;ol</option>
		<option value="Spanish/English">es &raquo; en</option>
		<option value="English/French">fran&ccedil;ais</option>
		<option value="French/English">fr &raquo; en</option>
		<option value="English/Italian">italiano</option>
		<option value="Italian/English">it &raquo; en</option>
		<option value="English/Norwegian">norsk</option>
		<option value="English/Portuguese">portug<!--u&ecirc;s--></option>
		<option value="Portuguese/English">pt &raquo; en</option>
	</select>
	<br />
	<input type="submit" value="SFA_TRADUCIR" />
	</small>
</form>
<!-- End Translate Form Template -->
