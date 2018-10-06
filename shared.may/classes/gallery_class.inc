<?

class Gallery extends Page
{

    // this class uses a lot of different templates. this is done so the result is still configurable.
    // also if our template class could handle if-else & various loops we could reduce the amount of
    // templates used.


    function Gallery($category_id, $year=0, $month=0)
    {
	// initialises the class. when vars are set, pages will be build.
	if($year && $month)
	{
	    // we can only automatically create the galleries if we know the month & year.
	    if($category_id)
	    {
		$checked_category = $category_id ;
	    }else{
		$checked_category = '';
	    }
	    $this->create_gallery($checked_category, $year, $month) ;
	}else{
	    // return something so the we don't get errors.
	    // we'll need this for render_this_month & render_all_pages
	    return 1 ;
	}
    }

    function create_gallery($category, $year, $month)
    {
	// if the cache_dirs & the index.php doesn't exist, we'll make them.
	// we could use $this->year & $this->month more in the code.
	$this->year = $year ;
	$this->month = $month ;
	$this->check_dirs($year, $month);
	$this->create_index_file($year, $month);

        if(!$category) { $catlink = ''; }
        else { $catlink = $category . '_' ; }

	// we'll do the same as the search lets you do. so it doesn't look different to people.
	$types = array('image', 'video', 'audio', 'application');
	foreach($types as $key)
	{
	    $result = $this->get_uploaded_files($category, $year, $month, $key);
	    if(is_array($result) && count($result) > 0)
	    {
		$result = $this->set_real_article_id($result);
		$result = $this->sort_array($result, "forced_id");
		$result = array_chunk($result, $GLOBALS['files_per_page']);	
		$total_pages = count($result);
		$page_counter = $total_pages ;
		while($page = array_pop($result))
	        {
		    $page_html = $this->render_single_page($page, $key, $page_counter, $total_pages);
		    $this->cache_file($page_html, SF_CACHE_PATH.'/galleries/'.$year.'/'.$month.'/'.$catlink.$key."_".$page_counter.".inc");
		    $page_counter--;
		}
	    }else{
		$page_html = $this->render_single_page('', $key, 1, '');
		$this->cache_file($page_html, SF_CACHE_PATH.'/galleries/'.$year.'/'.$month.'/'.$catlink.$key."_1.inc");
	    }
	}
    }

    function render_single_page($array, $mime_type, $page, $total_pages)
    {	
	// render the search
	$search = $this->render_search();

	// render the body & navigation.
	if($total_pages)
	{
	    $header = $this->render_nav($page, $total_pages, $key);
	    $body = $this->render_body($array, $mime_type);
	}else{
	    $header = '' ;
	    $body = $this->render_empty_body();
	}

	// render this ....
	$tpl = new FastTemplate(SF_TEMPLATE_PATH);
	$tpl->define(array(gallery => 'gallery.tpl'));
	$tpl_vars['TPL_TITLE'] = "<?php echo \$sftr->trans('media') ; ?> ";
	$tpl_vars['TPL_HEADER'] = $search.$header ;
	$tpl_vars['TPL_FOOTER'] = $header.$search ;
	$tpl_vars['TPL_ROWS'] =  $body;
	$tpl->assign($tpl_vars);
	$tpl->parse(MAIN, "gallery");	
	return $tpl->fetch(MAIN);
    }

    function render_body($article_array, $mime_type)
    {
	// we might need a better ordening method to avoid parts of the next line.
	$row_array = array_reverse(array_chunk($article_array, 4));
	$rows = '';
	while($row = array_pop($row_array))
	{
	    $rows .= $this->render_row($row, $mime_type) ;
	}
	// temp
	return $rows ;
    }

    function render_empty_body()
    {
	// we just cache a message that there aren't such files.
	// if we don't we'll eiter return to the database or don't cache wich will throw warnings.
	$tpl = new FastTemplate(SF_TEMPLATE_PATH);
	$tpl->define(array("empty" => "gallery_row_empty.tpl"));
	$tpl->assign(array("TPL_EMPTY" => "<?php echo \$sftr->trans('no_files'); ?>"));
	$tpl->parse('MAIN', "empty");
	return $tpl->fetch('MAIN');
    }

    function render_row($row= array(), $mime_type)
    {
	$tmpl = new FastTemplate(SF_TEMPLATE_PATH);
	$tmpl->clear_all();
	$tmpl->define(array(rows => "gallery_row.tpl"));
	$tmpl->define_dynamic("list", "rows");
	while($picture = array_shift($row))
	{
	    // set basic variables. like path, url of file, $ext & articlelink, title, ....
	    // basically all we need for all file types :-)
	    $path = SF_UPLOAD_PATH.'/'.$this->year.'/'.$this->month.'/'.basename($picture['linked_file']);
	    $url = SF_UPLOAD_URL.'/'.$this->year.'/'.$this->month.'/'.basename($picture['linked_file']);
	    eregi("([a-z0-9/\._-]+)\.([a-z0-9]+)$", $path, $regs);
	    $ext = $regs[2];
	    $var['TPL_FULL_SIZE'] = $url ;
	    $var['TPL_HEADING'] = $picture['heading'] ;
	    if($picture['parent_id'] == 0)
            {
                $var['TPL_ARTICLE'] = SF_NEWS_URL.'/'.$this->year.'/'.$this->month.'/'.$picture['forced_id'].'.php' ;
            }else{
                $var['TPL_ARTICLE'] = SF_NEWS_URL.'/'.$this->year.'/'.$this->month.'/'.$picture['forced_id'].'_comment.php#'.$picture['id'];
            }
	    $var['TPL_VIEW_ARTICLE'] = "<?php echo \$sftr->trans('read_article'); ?>" ;
	    $var['TPL_ALT'] = htmlentities($picture['heading']);

	    // we'll handle different media types differently.
	    switch($mime_type) 
	    {
	    // images.
		case "image":
		    $var['TPL_VIEW_MID'] = '' ;
		    if(file_exists($path.'mid.'.$ext))
		    {
			$var['TPL_VIEW_MID'] = "- <a href=\"".$url."mid.".$ext."\"><?php echo \$sftr->trans('view_midsize'); ?></a> ";
		    }
		    $var['TPL_VIEW_FILE'] = "<?php echo \$sftr->trans('view_fullsize') ; ?>" ;
		    $var['TPL_SOURCE'] = $url.'thumb.'.$ext;
		    $info = getimagesize($path.'thumb.'.$ext);
		    $var['TPL_SIZE'] = $info[3];
		    break;

	    // audio.
		case "audio":
		case "video":
		case "application":
		{
		    $var['TPL_VIEW_MID'] = '';
		    $var['TPL_VIEW_FILE'] = "<?php echo \$sftr->trans('download'); ?>";
		    if(ereg('video', $picture['mime_type']))
		    {
			$thumb = SF_ROOT_URL.'/im/gal_video.png';
			$thumbf = SF_WEB_PATH.'/im/gal_video.png';
		    }
		    elseif(ereg('pdf', $picture['mime_type']))
		    {
			$thumb = SF_ROOT_URL .'/im/gal_pdf.png';
			$thumbf = SF_WEB_PATH.'/im/gal_pdf.png';
		    }else{
			$thumb = SF_ROOT_URL .'/im/gal_default.png';
			$thumbf = SF_WEB_PATH.'/im/gal_default.png';
		    }
		    $var['TPL_SOURCE'] = $thumb;
		    $info = getimagesize($thumbf);
		    $var['TPL_SIZE'] = $info[3];
		    break;
		}
	    }
	    $tmpl->assign($var);
	    $tmpl->parse(LISTS, ".list");
	}
	$tmpl->parse(MAIN, "rows");
	$row_html = $tmpl->fetch('MAIN');
	return $row_html ;
    }


    function render_nav($page, $total_pages, $mime_type)
    {
	$prev_page = $page -1 ;
	$next_page = $page +1 ;
	if($page == 1) 
	{
	    $tpl_var['TPL_PREVIOUS'] = '';
	}else{
	    $tpl_var['TPL_PREVIOUS'] = "<a href=\"index.php?page=".$prev_page."\"><?php echo \$sftr->trans('previous'); ?></a>";
	}

	if(($page == $total_pages) || !$total_pages)
	{
	    $tpl_var['TPL_NEXT'] = '';
	}else{
	    $tpl_var['TPL_NEXT'] = "<a href=\"index.php?page=".$next_page."\"><?php echo \$sftr->trans('next') ; ?></a>";
	}
	$tpl_var['TPL_PAGES_LINK'] = '- ';
	for($i = 1; $i < $total_pages + 1; $i++)
	{
	    if($i == $page)
	    {
		$tpl_var['TPL_PAGES_LINK'] .= $page . ' - ';
	    }else{
		$tpl_var['TPL_PAGES_LINK'] .= '<a href="index.php?page='.$i.'">'.$i.'</a> - ';
	    }
	}
	$tpl = new FastTemplate(SF_TEMPLATE_PATH) ;
	$tpl->clear_all();
	$tpl->define(array("header" => "gallery_nav.tpl"));
	$tpl->assign($tpl_var);
	$tpl->parse(MAIN, "header");
	return $tpl->fetch("MAIN");
    }

    function render_search()
    {
	return file_get_contents(SF_TEMPLATE_PATH.'/gallery_search.tpl');
    }
    
    function get_uploaded_files($category, $year, $month, $mime_type)
    {
	/*
	    We'll need te following variables:
	    1. list of linked_files
	    2. their mime_type
	    3. their_id & parent_id
	    4. the title of the article (to give some metainfo together with the file.
	*/
	$db = new DB;
	if(!$category)
	{
	    $sql = "select id, parent_id, heading, mime_type, linked_file from webcast where length(linked_file) > 0 and created like";
	    $sql .= " '".$year.$month."%' and display != 'f' and mime_type like '%".$mime_type."%' order by id desc";
	}else{
	    $sql = "select w.id, w.parent_id, w.heading, w.mime_type, w.linked_file from webcast w, catlink c where length(w.linked_file) > 0 ";
	    $sql .= "and w.id = c.id and c.catid = '".$category."' and w.created like '".$year.$month."%' and display != 'f' and mime_type like '%".$mime_type."%' order by id desc";
	}
	return $db->query($sql);
    }

    function sort_array(&$matrix,$sortKey)
    {
        foreach ($matrix as $key => $subMatrix)
        {
                $value = $subMatrix[$sortKey];
                $tmpArray[$key] = $value ;
        }
        arsort($tmpArray);
        foreach($tmpArray as $key=>$value)
        {
                $array[$count] = $matrix[$key] ;
                $count ++;
        }
        return $array ;
    }

    function set_real_article_id($array)
    {
	$count = 0 ;
	while($row = array_pop($array))
	{
	    if($row['parent_id'])
	    {
		$row['forced_id'] = $row ['parent_id'] ;
	    }else{
		$row['forced_id'] = $row['id'];
	    }
	    $new_array[$count] = $row ;
	    $count ++;
	}
	return $new_array;
    }

    function create_index_file($year, $month)
    {
	/*  This function will create an index.php file 
	    in the SF_UPLOAD_PATH.'/yyyy/mm/' directory if that
	    file doesn't exist yet
	    maybe this should be build from a template ?
	*/
	if(!file_exists(SF_UPLOAD_PATH.'/'.$year.'/'.$month.'/index.php'))
	{
	    $file .= "<?php\n";
	    $file .= "include_once(\"shared/global.cfg\");\n";
	    $file .= "\$sftr = new Translate ; \n";
	    $file .= "\$sftr->create_translate_table('gallery');\n\n";
	    $file .= "\$year = \"$year\";\n";
	    $file .= "\$month = \"$month\";\n\n";
	    $file .= "sf_include_file(SF_INCLUDE_PATH, 'content-header.inc');\n\n";
	    $file .= "    if(!\$_GET['page']) { \$page = 1 ; }\n";
	    $file .= "    else { \$page = \$_GET['page'] ; }\n\n";
	    $file .= "    if(!\$_GET['medium']) { \$medium = \"image_\" ; }\n";
	    $file .= "    else { \$medium = \$_GET['medium'].\"_\"; }\n\n";
	    $file .= "    if(!\$_GET['category_id']) { \$catlink = '' ; } \n";
	    $file .= "    else { \$catlink = \$_GET['category_id'] . '_' ;}\n\n";
	    $file .= "include_once(SF_CACHE_PATH.\"/galleries/\$year/\$month/\$catlink"."\$medium"."\$page".".inc\");\n\n";
	    $file .= "sf_include_file(SF_INCLUDE_PATH,'content-footer.inc');\n\n";
	    $file .= "?>";
	    $this->cache_file($file, SF_UPLOAD_PATH.'/'.$year.'/'.$month.'/index.php');
	}
    }

    function check_dirs($year, $month)
    {
	// will create the directories of a certain month if they don't exist yet.
	if(!is_dir(SF_CACHE_PATH.'/galleries/'.$year))
	{
	    mkdir(SF_CACHE_PATH.'/galleries/'.$year, 0777);
	}
        if(!is_dir(SF_CACHE_PATH.'/galleries/'.$year.'/'.$month))
        {
            mkdir(SF_CACHE_PATH.'/galleries/'.$year.'/'.$month, 0777);
        }
    }

    function render_this_month()
    {
	/*
	    this will create ALL your galleries for the current month.
	    (including the galleries for all categories).
	*/
	$this->tr = new Translate ;
        include_once(SF_CACHE_PATH.'/cat_options.inc') ;
	$month = date('m');
	$year =  date('Y');
	foreach($cat_options as $key => $value)
	{
	    $this->create_gallery($key, $year, "$month");
	}
	$this->create_gallery('', $year, $month);
    }
	

    function render_all_pages()
    {
	/*
	    this will render ALL pages. if you wish to do only the ones for this month, use
	    render_this_month. you can do specific ones directly when calling the class.
	*/    
	$m = $GLOBALS['site_start_month'] ;
	$y = $GLOBALS['site_start_year'] ;
	$mn = date('m');
	$yn = date('Y') + 1 ;
	$this->tr = new Translate ;
	include_once(SF_CACHE_PATH.'/cat_options.inc') ;

	for($y; $y < $yn; $y++)
	{
	    $realm = 1 ;
	    if($y == $GLOBALS['site_start_year']) $realm = $GLOBALS['site_start_month'] ;
	    $maxmonth = 13;
	    if($y == date('Y')) { $maxmonth = date('m') + 1;}
	    for($realm; $realm < $maxmonth; $realm ++)
	    {
		if($realm < 10) { $newm = "0$realm"; }
		else { $newm = $realm ;}
		foreach($cat_options as $key => $value)
		{
		    if($key != '')
		    {
			$this->create_gallery($key, "$y", "$newm");
		    }
		}
		$this->create_gallery('', "$y", "$newm");
	    }
	}
    }

}
?>
