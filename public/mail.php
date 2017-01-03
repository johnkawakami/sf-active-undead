<?php
// Mail an Article
header($_SERVER['SERVER_PROTOCOL'].' 403 Forbidden');
header("Status: 403 Forbidden");
print '';
exit; // mail is disabled for now

include_once("shared/global.cfg");
$page = new Page('mailable');

if ($page->get_error())
{
    echo "Fatal error: " . $page->get_error();
} else
{
    $page->build_page();
    echo $page->get_html();
}

?>
<p>
	<small>
		<?php include(SF_INCLUDE_PATH . '/disclaimer.inc'); ?>
	</small>
</p>

