<?php

include_once(SF_SHARED_PATH . '/classes/date_class.inc');

$date = new Date;
$date->set_time_zone();
$fdate = $date->get_formatted_date();
$ftime = $date->get_formatted_time();

$username = $_SESSION['username'];

if ($GLOBALS['time_diff'] > 0)
{
    $tz = "+" . $GLOBALS['time_diff'];
} else
{
    $tz = $GLOBALS['time_diff'];
}


?>
<HTML>
<HEAD>
<TITLE>sf-active admin</TITLE>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="/themes/admin.css" />
</HEAD>

<BODY BGCOLOR="#ffffff" TEXT="#000000">

<!-- main table for page -->
<TABLE WIDTH="100%" BORDER="1" CELLSPACING="2" CELLPADDING="2">
<TR><TD BGCOLOR="#FFCC00">
<FONT FACE="MS Sans Serif, Verdana, Arial, sans-serif" SIZE="-2">
<B><FONT FACE="Verdana, Arial, Helvetica, sans-serif" SIZE="-1">
<?php echo $GLOBALS['site_nick'] ?> : <?php echo $sftr->trans('adminsite'); ?> : 
<A CLASS="bar" HREF="/admin/authentication/authenticate_display_logon.php"><?php echo $sftr->trans('adminlogout'); ?></A>
: <A CLASS="bar" TARGET="_blank" HREF="<?php echo SF_ROOT_URL ?>"><?php echo $sftr->trans('adminviewsite'); ?></A></FONT></B></FONT>
</TD></TR>

<TR><TD width="30%" BGCOLOR="#eeeeee">

<!-- navigation table -->
<TABLE WIDTH="100%" BORDER="0" CELLSPACING="0" CELLPADDING="1">
<TR><TD VALIGN="TOP">

<TABLE WIDTH="100%" BORDER="0" CELLSPACING="0" CELLPADDING="0" BGCOLOR="#666666">
<TR><TD>

<TABLE WIDTH="100%" BORDER="0" CELLSPACING="1" CELLPADDING="4">

<TR> 
<TD BGCOLOR="#e0e0e0"><A CLASS="nav" HREF="http://sfactive.indymedia.org/docs/">DOCS</A></TD>
</TR>


<TR> 
<TD BGCOLOR="#e0e0e0"><A CLASS="nav" HREF="/admin/category/category_display_list.php"><?php echo $sftr->trans('admincategories'); ?></A></TD>
</TR>

<tr>
  <td bgcolor="#e0e0e0"><A CLASS="nav" HREF="/admin/language/language_display_list.php"><?php echo $sftr->trans('adminlanguage'); ?></a></td>
</tr>

<TR>
<TD BGCOLOR="#e0e0e0"><A CLASS="nav" HREF="/admin/article/"><?php echo $sftr->trans('adminarticles'); ?></A></TD>
</TR>

<TR>
<TD BGCOLOR="#e0e0e0"><A CLASS="nav" HREF="/admin/qc/">REPORTED POSTS</A></TD>
</TR>

<TR>
<TD BGCOLOR="#e0e0e0"><A CLASS="nav" HREF="/admin/calendar/"><?php echo $sftr->trans('admincalendar'); ?></A></TD>
</TR>

<TR>
<TD BGCOLOR="#e0e0e0"><FONT FACE="Verdana, Arial, Helvetica, sans-serif" SIZE="-2"><?php echo $sftr->trans('adminxml'); ?></FONT></TD>
</TR>

<TR> 
<TD BGCOLOR="#e0e0e0"><A CLASS="nav" HREF="/admin/upload/upload_display_add.php"><?php echo $sftr->trans('adminupload'); ?></A></TD>
</TR>

<tr>
<td bgcolor="#e0e0e0"><a class="nav" href="/admin/dossier/index.php"><? echo $sftr->trans('admindossier'); ?></a></td>
</tr>

<TR>
<TD BGCOLOR="#e0e0e0"><A CLASS="nav" HREF="/admin/user/user_display_list.php"><?php echo $sftr->trans('adminusers'); ?></A></TD>
</TR>

<TR>
<TD BGCOLOR="#e0e0e0"><A CLASS="nav" HREF="/admin/email/admin_email.php"><?php echo $sftr->trans('adminemail'); ?></A></TD>
</TR>

<TR> 
<TD BGCOLOR="#e0e0e0"><FONT FACE="Verdana, Arial, Helvetica, sans-serif" SIZE="-2"><A class="nav" href="/webalizer/"><?php echo $sftr->trans('adminstats'); ?></A></FONT></TD>
</TR>

<TR> 
<TD BGCOLOR="#e0e0e0"><A CLASS="nav" HREF="/admin/log/index.php"><?php echo $sftr->trans('admingraphics'); ?></A></TD>
</TR>

<TR> 
<TD BGCOLOR="#e0e0e0"><FONT FACE="Verdana, Arial, Helvetica, sans-serif" SIZE="-2"><A class="nav" href="/admin/spam/"><?php echo $sftr->trans('adminspam'); ?></A></FONT></TD>
</TR>

<TR>
<TD BGCOLOR="#e0e0e0"><FONT FACE="Verdana, Arial, Helvetica, sans-serif" SIZE="-2"><A class="nav" href="/admin/log/file_viewer.php?filename=comments_latest.inc"><?php echo $sftr->trans('adminlatestcomments'); ?></A></FONT></TD>
</TR>

<tr><td bgcolor="#e0e0e0"><a class="nav" href="/admin/cities/"><?php echo $sftr->trans('admincities'); ?></a></td></tr>

<TR>
<TD BGCOLOR="#e0e0e0"><FONT FACE="Verdana, Arial, Helvetica, sans-serif" SIZE="-2">
<?php echo $sftr->trans('adminloggedin'); ?>: <FONT COLOR="#990000"><?php echo $username ?></FONT></FONT></TD>
</TR>

<TR> 
<TD BGCOLOR="#e0e0e0"><FONT FACE="Verdana, Arial, Helvetica, sans-serif" SIZE="-2">
<?php echo $fdate; ?><BR><?php echo $ftime; ?><BR>GMT<?php echo $tz; ?></FONT></TD>
</TR>

<TR>
<TD BGCOLOR="#e0e0e0"><A CLASS="nav" HREF="/admin/authentication/authenticate_display_logon.php"><?php echo $sftr->trans('adminlogout'); ?></a></TD>
</TR>

</TABLE>

</TD></TR></TABLE>
</TD>

<TD width="100%" VALIGN="TOP">
<TABLE WIDTH="100%" BORDER="0" CELLSPACING="6" CELLPADDING="6">
<TR>
<TD>

