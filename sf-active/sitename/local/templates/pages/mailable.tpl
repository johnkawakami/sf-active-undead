<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <title>
			TPL_PRN_TITLE
        </title>
        <style type="text/css">
        /*<![CDATA[*/
        <!--
                .heading { font-family: sans-serif }
                .summary { font-family: sans-serif }
                .media { font-family: sans-serif }
                .article { font-family: serif }
                .link { font-family: sans-serif }
                .addcomment { visibility: hidden; display: none; }
        // -->
        /*]]>*/
        </style>
    </head>
    <body>
        <h3>
            <b>TPL_LANG_PRN_SITE_NAME</b>
        </h3>
        <hr />
<b>TPL_ERROR_MSG</b>
<p>
<form method="post" name="direction">
<b>TPL_FROMADDRESS</b>: <input type="text" name="fromaddress" value="TPL_FROMVALUE"><br>
<b>TPL_TOADDRESS</b>: <input type="text" name="toaddress" value="TPL_TOVALUE"><br>
<b>TPL_SUBJECT</b>: <input type="text" name="subject" value="TPL_SUBJECTVALUE"><br>
<b>TPL_COMMENTMAIL</b>: <br>
<textarea name="commentmail" cols="50" rows="5">TPL_COMMENTMAIL_VALUE</textarea><br>
<input type="hidden" value="TPL_IDVALUE" name="id">
<input type="hidden" value="TPL_COMMENTSVALUE" name="comments">
<input type="submit" name="email_it" value="TPL_SUBMIT_EMAIL"><br>
</form>
</p>


        <p>
            TPL_LANG_PRN_ORGINAL_ARTICLE <a href="TPL_PRN_ARTICLE_URL">TPL_PRN_ARTICLE_URL</a> <a href="TPL_PRN_COMMENT_LINK">TPL_PRN_COMMENT_TEXT</a>
        </p>
        TPL_PRN_ARTICLE_HTML 
        <hr />
        TPL_PRN_COMMENTS_HTML
    </body>
</html>

