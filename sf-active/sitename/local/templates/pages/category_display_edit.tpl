<A href="../index.php">TPL_ADMIN_INDEX</A> :
<a href="category_display_list.php">TPL_CATEGORIES</a>

<h3>TPL_SUBTITLE</h3>

<form name ="category_update_form" method="post" action="TPL_LOCAL_FORM">

<TABLE>
    <TR> 
    	<TD colspan="2">
	  <input type=submit name="save2" value="TPL_SAVE" />
	</TD>
    </TR>

	  <INPUT TYPE="hidden" NAME="parentid" VALUE=1>
	  <INPUT TYPE="hidden" NAME="category_id" VALUE=TPL_LOCAL_CATEGORY_ID>

    <TR> 
      	<TD>TPL_CATEGORY_SHORTNAME (<b>TPL_REQUIRED</b>)</TD>
       	<TD> 
          <input name="shortname" value="TPL_LOCAL_CATEGORY_SHORTNAME" />
       	</TD>
    </TR>
    <TR>
	<TD>TPL_CATEGORY_NAME</TD>
   	<TD>
   	  <INPUT NAME="name" VALUE="TPL_LOCAL_CATEGORY_NAME" />
   	</TD>
    </TR>
    <TR>
   	<TD>TPL_ORDERNUM</TD>
   	<TD><INPUT NAME="order_num" VALUE="TPL_LOCAL_ORDERNUM" /></TD>
    </TR>
    <TR>
	<TD>TPL_CATEGORY_SUMMARYLENGTH</TD>
	<TD><input name="summarylength" value="TPL_LOCAL_SUMMARYLENGTH" /></TD>
    </TR>
    <TR>
	<TD>TPL_TEMPLATE_NAME</TD>
	<TD><INPUT NAME="template_name" VALUE="TPL_LOCAL_TEMPLATE_NAME" /></TD>
    </TR>
    <TR>
	<TD>TPL_DEFAULT_TEMPLATE</TD>
	<TD>TPL_LOCAL_SELECT_TEMPLATE</TD>
    </TR>
    <TR> 						
	<TD>TPL_NEWSWIRE_TYPE</TD>
	<TD>TPL_LOCAL_SELECT_NEWSWIRE</TD>
    </TR>
    <TR>
	<TD>TPL_CENTERCOLUMN_TYPE:</TD>
	<TD>TPL_LOCAL_SELECT_CENTERCOLUMN</TD>
    </TR>
    <TR>
	<TD>TPL_CATCLASS_TYPE:</TD>
	<TD>TPL_LOCAL_SELECT_CATCLASS</TD>
    </TR>
    <TR> 
        <TD valign="top">TPL_CATEGORY_DESCRIPTION:</TD>
        <TD><textarea name="description" cols="40" rows="5" wrap="PHYSICAL">TPL_LOCAL_DESCRIPTION</textarea></TD>
    </TR>
    <TR> 
        <TD><input type="checkbox" name="recache" value="true" /></TD>
        <TD valign="top">TPL_RECACHE</TD>
    </TR>
<TR> 
        <TD colspan="2"> 
          <input type=submit name="save" value="TPL_SAVE"/>
        </TD>
    </TR>
    <TR></TR>
</TABLE>
</FORM>

