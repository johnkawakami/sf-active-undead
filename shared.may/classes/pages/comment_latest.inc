<?php

// Class for comment_latest

class comment_latest extends Page
{
    function comment_latest()
    {
        // Class constructor, does nothing
        return 1;
    }

    function execute()
    {
        $cache_file_name = SF_CACHE_PATH . '/comments_latest.inc';
        $file_rows = file($cache_file_name);
        $tblhtml = '';

        while (list($id,$text) = each($file_rows))
        {
            $tblhtml .= $text;
        }

        $tblhtml .= $text . '';
        $this->tkeys['LOCAL_COMMENT_ROWS'] = $tblhtml;
        $this->tkeys['LAST_UPDATED']= date("r",filemtime("$cache_file_name"));
        return 1;
    }

	function add_comment_db( $parent_numcomment, $parent_heading, $comment_link, $parent_id, $comment_author, $comment_id, $parent_postingdate)
	{
		($parent_id = filter_var( $parent_id, FILTER_VALIDATE_INT )) || die('bad parent id');
		($comment_id = filter_var( $comment_id, FILTER_VALIDATE_INT )) || die('bad comment id');
		($parent_numcomment = filter_var( $parent_numcomment, FILTER_VALIDATE_INT )) || die('bad parent numcomment');
		// the heading might be sanitized, so remove slashes then add them again.
		$parent_heading = addslashes( stripslashes( $parent_heading ));
		$comment_author = addslashes( stripslashes( $comment_author ));
		($comment_link = filter_var( $comment_link, FILTER_VALIDATE_URL )) || die('bad comment link');

		$db = new \SFACTIVE\DB;
		$res = $db->query( $q=
			"INSERT INTO commentlist ( parent_id, comment_id, heading, author, url, comments, postingdate, parentpostingdate ) 
			VALUES ( $parent_id, $comment_id, '$parent_heading', '$comment_author', '$comment_link', 
					1, NOW(), '$parent_postingdate' ) 
			ON DUPLICATE KEY UPDATE
			comment_id=$comment_id, heading='$parent_heading',
			author='$comment_author', url='$comment_link',
			comments=$parent_numcomment, postingdate=NOW(), parentpostingdate='$parent_postingdate'
			");
		$res = $db->query( "DELETE FROM commentlist WHERE postingdate < DATE_SUB( NOW(), INTERVAL 3 MONTH)" );
	}

    function add_latest_comment($parent_numcomment, $parent_heading, $comment_link, $parent_id, $comment_author, $comment_id, $parent_postingdate)
    {
		global $time_diff;

		$this->add_comment_db( $parent_numcomment, $parent_heading, $comment_link, $parent_id, $comment_author, $comment_id, $parent_postingdate);

        $tr = new Translate;

        $comments_file = SF_CACHE_PATH . '/comments_latest.inc';
        $fc = fopen($comments_file,"r");

        // note that the regexps below are dependent upon html
        // maybe this can be fixed by declaring a class for the table instead of each cell?
        $c = 0;

        if ($parent_numcomment > 1)
        {
            $commentext = $tr->trans('plural_comment');
        } else
        {
            $commentext = $tr->trans('single_comment');
        }

        $td = "<td class=\"bgsearchgrey\">";
        $comment_link .= "#" . $comment_id;
        $article_link = "<a href=\"$comment_link\">" . $parent_heading . "</a>";
        $article_link .= " <small>($parent_numcomment $commentext)</small>";
// edit to correct time diff errors
        //$carray[] = $parent_id . "|" . $article_link . "|" . $comment_author . "|" . date("g:ia D M j");
	$time_date=time()+$time_diff;
	$date=date("g:ia D M j", $time_date);
        $carray[] = $parent_id . "|" . $article_link . "|" . $comment_author . "|" . $date;

        while (!feof($fc))
        {
            $strbuffer = fgets($fc,8192);
            $strbuffer = preg_replace("/|/","",chop($strbuffer));
            $commentid = preg_replace("/<!--\s(\d+)\s-->(.|\s)*/","$1",$strbuffer);
            $commentinfo = preg_replace("/(.*)grey\">(.*?)<\/td><td class=\"bgsearchgrey\">(.*?)<\/td><td class=\"bgsearchgrey\">(.*?)<\/td>(.*|\s)/","$2|$3|$4",$strbuffer);
            $carray[] = $commentid."|".$commentinfo;
        }

        reset($carray);

        while ($comment = array_shift($carray))
        {
            list($comid,$title,$author,$date) = explode("|",$comment);
            if (($c < 1 || $comid != $parent_id) && $comid > 0)
            {
                $comment_result.="<!-- ".$comid." --><tr>".$td.$title."</td>".$td.$author."</td>".$td.$date."</td></tr>\n";
            }
            $c++;
            if ($c >= 50) break;
        }

        fclose($fc);

        if ($comment_result)
        {
            $fd = fopen($comments_file,"w+");
            fwrite($fd,$comment_result);
            fclose($fd);
        }
    }
}

?>
