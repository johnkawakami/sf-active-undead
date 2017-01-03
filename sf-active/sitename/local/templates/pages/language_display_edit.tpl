<A href="../index.php">TPL_ADMIN_INDEX</A> :
<a href="/admin/language/language_display_list.php">TPL_LANGUAGES</a>

<h3>TPL_SUBTITLE</h3>

<form name ="language_update_form" method="post" action="TPL_LOCAL_FORM">

<table>
    <tr>
    	<td colspan="2">
	  <input type=submit name="save2" value="TPL_SAVE" />
	</td>
    </tr>

	  <input type="hidden" name="id" value="TPL_ID">

    <TR> 
      	<TD>TPL_LANGUAGE_NAME</TD>
       	<TD> 
          <input name="name" value="TPL_LOCAL_NAME" />
       	</TD>
    </TR>
    <TR>
	<TD>TPL_LANGUAGE_CODE</TD>
   	<TD>
   	  <INPUT NAME="language_code" VALUE="TPL_LOCAL_LANGUAGE_CODE" />
   	</TD>
    </TR>
    <tr>
	  <td>TPL_ORDER_NUM</td>
	  <td><input name="order_num" value="TPL_LOCAL_ORDER_NUM" /></td>
    </tr>
    <tr>
	<td>TPL_DISPLAY</td>
	<td>TPL_LOCAL_DISPLAY
	</td>
    <tr> 
    <tr>
	<td colspan="">
	    TPL_BUILD_SITE
	</td>
	<td>
	    TPL_CHECKBOX_BUILD<br />
	    TPL_BUILD_INFO
 	</td>
    </tr>

        <TD colspan="2"> 
          <input type=submit name="save" value="TPL_SAVE"/>
        </TD>
    </TR>
    <TR></TR>
</TABLE>
</FORM>

