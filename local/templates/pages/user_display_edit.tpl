<A href="../index.php">TPL_ADMIN_INDEX</A> :
<A href="user_display_edit.php">TPL_USER_ADD</A><br /><br />

<div>
	TPL_LOCAL_SEC_WARNING
	TPL_LOCAL_PASS_DESC
	TPL_LOCAL_PASS_DESC2
</div>

<Form name="user_edit" method="post" action="TPL_LOCAL_FORM_ACTION">
TPL_LOCAL_FORM_HIDDEN

<TABLE>

<TR><TD>TPL_USERNAME</TD><TD>
<INPUT NAME="username1" VALUE="TPL_LOCAL_USERNAME" />
</TD></TR>

<TR><TD>TPL_PASSWORD</TD><TD>
<INPUT NAME="password" type="PASSWORD" VALUE="" />
</TD></TR>

<TR><TD>TPL_EMAIL</TD><TD>
<INPUT NAME="email" VALUE="TPL_LOCAL_EMAIL" />
</TD></TR>

<TR><TD>TPL_PHONE</TD><TD>
<INPUT NAME="phone" VALUE="TPL_LOCAL_PHONE" />
</TD></TR>

<TR><TD>TPL_FIRST_NAME</TD><TD>
<INPUT NAME="first_name" VALUE="TPL_LOCAL_FIRST_NAME" />
</TD></TR>

<TR><TD>TPL_LAST_NAME</TD><TD>
<INPUT NAME="last_name" VALUE="TPL_LOCAL_LAST_NAME" />
</TD></TR>

<TR><TD>TPL_LASTLOGIN</TD>
<TD>TPL_LOCAL_LASTLOGIN</TD></TR>

</TABLE>

<INPUT type=submit name="save" value="TPL_SAVE" />

</TD></TR></TABLE>

</FORM>
