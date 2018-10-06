<?php // vim:et:ai:ts=4:sw=4
class archive_index extends Page
{
    function archive_index()
    {
        return 1;
    }
    function execute()
    {
        include_once(SF_SHARED_PATH.'/classes/category_class.inc');
        $category_obj=new SFACTIVE\Category;
        $fields=$category_obj->get_center_list();
        while ($row=array_pop($fields))
        {
            $contents .= '<div><b><big><a href="';
            if ($row['category_id']==$GLOBALS['config_defcategory'])
            {
                $contents .= '/">';
            } else {
                $contents .= SF_FEATURE_URL.'/'.$row['shortname'].'/">';
            }
            $contents .= $row['name'].'</a></big></b></div>';
            if ($row['newswire']!='n')
            {
                $contents .= '<li><a href="'.SF_NEWS_URL.'/?category=';
                if ($row['category_id']!=$GLOBALS['config_defcategory'])
                {
                    $contents .= $row['category_id'];
                }
                $contents .= '">'.$GLOBALS['dict']['newswire'].'</a></li>';
            }
            $contents .= '<li><a href="'.SF_ARCHIVE_URL.'/archive_display_by_date.php?category_id='.$row['category_id'].'">'.$GLOBALS['dict']['archive_date'].'</a></li>';
            $contents .= '<li><a href="'.SF_ARCHIVE_URL.'/archive_display_list.php?category_id='.$row['category_id'].'">'.$GLOBALS['dict']['archive_id'].'</a></li>';
            $contents .= '<li><a href="'.SF_ARCHIVE_URL.'/archive_display_calendar.php?category_id='.$row['category_id'].'">'.$GLOBALS['dict']['archive_week'].'</a></li>';
            $contents .= '<small><br /></small>';
        }
        $db_obj = new SFACTIVE\DB;
        $page = $_GET['page'];
        $current = $_GET['current'];
        $keyword = $_GET['keyword'];
        if ($page < 1) $page = 1;
        $limit = 20;
        $offset = ($page - 1) * $limit;
        $last=($page - 1);   
        $next=($page + 1);
        $query = "select s.category_id as category_id,s.title1 as title1,s.title2 as title2,";
        $query .= "date_format(s.creation_date,'%Y-%m-%d') as date,s.feature_id as feature_id,c.name as name "; 
        $query .= "from feature s,category c where ( s.status='a' ";
        if ($current) $query .= "or s.status='c' ";
        $query .= ") and s.is_current_version=1 ";
	// aggiungo ricerca in heading oltre che in summary
        if ($keyword) $query .= "and (summary like '%$keyword%' or title1 like '%$keyword%' or title2 like '%$keyword%') ";
        $query .= "and s.category_id=c.category_id order by s.creation_date desc limit $offset,$limit"; 
        $result=$db_obj->query($query);
        $numrows=sizeof($result);
        while ($row = array_pop($result))
        {
            $latest .= '<div style="padding: 2pt"><small><b>['.$row['name'].']</b></small>'
                     . ' <a href="/archives/archive_by_id.php?id='.$row['feature_id']
                     . '&amp;category_id='.$row['category_id'].'"><small><i>'.$row['title1'];
            $latest .= ':</i></small> <b>'.$row['title2'].'</b></a> <small>'.$GLOBALS['dict']['created'];
            $latest .= ' '.$row['date'].'</small></div>';
        }
        $this->tkeys['contents']=$contents;
        $this->tkeys['latest']=$latest;
        if ($page > 1) { $this->tkeys['last_link']='<a href="'.SF_ARCHIVE_URL.'/?page='.$last.'&amp;current=';
        $this->tkeys['last_link'].=$current.'&amp;keyword='.urlencode($keyword).'">&lt;&lt;</a>'; }
        else { $this->tkeys['last_link']=''; }
        if ($numrows==$limit) { $this->tkeys['next_link']='<a href="'.SF_ARCHIVE_URL.'/?page='.$next.'&amp;current=';
        $this->tkeys['next_link'].=$current.'&amp;keyword='.urlencode($keyword).'">&gt;&gt;</a>'; }
        else { $this->tkeys['next_link']=''; }
        $this->tkeys['page']=$page;
        $this->tkeys['site_crumb']=$GLOBALS['site_crumb'];
        if (!$current) {
            $this->tkeys['current_link']='<p><small><a href="'.SF_ARCHIVE_URL.'/?page='.$page.'&amp;keyword='.urlencode($keyword).'&amp;current=true">'.$GLOBALS['dict']['view_current'].'</a></small></p>';
        } else {
            $this->tkeys['current_link']='';
        }
        if (!$keyword) { $this->tkeys['keyword_set']=$GLOBALS['dict']['search_by_keyword']; }
        else { $this->tkeys['keyword_set']=$keyword; }
        $this->tkeys['current_set']=$current;
	$GLOBALS['page_title']=$GLOBALS['dict']['archive_index'];
    }
}
?>
