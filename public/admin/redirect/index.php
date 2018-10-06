<?php // vim:ai:et:sw=4:ts=4 
include 'shared/vendor/autoload.php';

/*
 * Redirect from an article to an arbitrary web page.
 * Part of the anti-spamming effort. Hijack clicks and turn
 * them into money or something.
 *
 * This is the admin home page.
 */
include('init.php');

$q = "SELECT id, url, last_used, redirects FROM redirect";
$stmt = $pdo->prepare($q);
$stmt->execute();
$all = $stmt->fetchAll();

function unhtmlentities($str)
{
	$str = preg_replace('/&quot;/',"'",$str);
	$str = preg_replace('/</','&lt;',$str);
	return $str;
}

function _table($content, $options=null) 
{
    $out = "<table>";
    if (is_callable($content)) {
        $out .= call_user_func($content);
    } else {
        $out .= $content;
    }

    $out .= "</table>";
    return $out;
}

function _row($row) 
{
    $id = $row['id'];
    $out = '<tr>'.
    _td($row['id']).
    _td(_url($row['url'])).
    _td($row['last_used']).
    _td($row['redirects']);
    if ($row['redirects']<10) {
        $out .= _td("<form action=delete.php method=post><input type=hidden name=id value=$id /><button>delete</button></form>");
    }
    $out .= '</tr>';
    return $out;
}

function _td($cell) 
{
    return "<td>$cell</td>";
}

function _url($url) {
    return "<a href='$url' target='_blank'>$url</a>";
}


?>
<? include("../admin_header.inc"); ?>
<b>Redirects</b>
<form action="add.php" method="post">
id: <input type="text" name="id" size="5" />
url: <input type="text" name="url" size="100" />
<input type="submit" />
</form>
<?
reset($all);
echo _table(function() use ($all) {
    return implode('',array_map('_row',$all));
});
?>

<? include('../admin_footer.inc'); ?>
