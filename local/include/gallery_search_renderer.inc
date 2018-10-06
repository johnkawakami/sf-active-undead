<?

// this will render the search you'll see on gallery_pages
$sftr->create_translate_table('newswire');
$this->tr = $sftr;
include_once(SF_CACHE_PATH.'/cat_options.inc');
include_once(SF_CLASS_PATH.'/calendar/date_renderer.inc');
include_once(SF_CLASS_PATH.'/calendar/event_renderer.inc');
include_once(SF_CLASS_PATH.'/calendar/default_object.inc');
$cache = new Cache ;
$event_renderer = new EventRenderer ;
$date_renderer = new DateRenderer ;
$GLOBALS['dropdown_max_year']=date(Y)+1;

        $medium_options = array(
                  "" =>  $sftr->trans('medium'),
                  "image" =>  $sftr->trans('image'),
                  "audio" =>  $sftr->trans('audio'),
                  "video" =>  $sftr->trans('video'),
                  "application" =>  $sftr->trans('other')
        );

	$search_medium = $cache->make_select_form('medium', $medium_options, $_GET['medium']);
	
	$search_category = $cache->make_select_form('category_id', $cat_options, $_GET['category_id']);

        $search_year = "<select name=\"year\">\n";
        $search_year .="<option value=''>".$sftr->trans('year')."</option>";
        $search_year .= $event_renderer->create_dropdown($date_renderer->get_years(),$year);
        $search_year .= "</select>";

        $search_month = "<select name=\"month\">\n";
        $search_month .="<option value=''>".$sftr->trans('month')."</option>";
        $search_month .= $event_renderer->create_dropdown($date_renderer->get_months(),$month);
        $search_month .= "</select>";
?>
<form method="get" action="<?php echo(SF_UPLOAD_URL.'/') ; ?>">
    <?php echo($search_year." ".$search_month." ".$search_medium." ".$search_category); ?> <input type="submit" name="submit" value="<?php echo($sftr->trans('go')); ?>" />
</form>
