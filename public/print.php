<?php
// Print View of a Article

include_once("shared/global.cfg");
$page = new Page('printable');

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

