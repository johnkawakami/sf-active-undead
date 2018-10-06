<?php // vim:et:ai:ts=4:sw=4
namespace SFACTIVE;

class Category extends \Cache
{
    //The Category class represents a feature. Unfortunately the naming conventions
    //in the HTML and code are different so a Category in teh code is a Feature in the HTML
    //and a story in teh html is a Fetaure in the code. Thsi should be changed ina future release
    //but will require DB changes/migration in addition to code changes.        

    function get_category_fields($category_id)
    {
        //Returns a dictionary object containing all fields for a given feature.

        $db_obj = new DB();

        if (strlen($category_id) != 0)
        {
            $category_detail_query = "SELECT * FROM category WHERE category_id=" . $category_id;
            $resultset = $db_obj->query($category_detail_query);
            $category = array_pop($resultset);
            return $category;
        } else
        {
            return 1;
        }
    }
        function get_category_array()
        {
            // Returns an array with category ID as key, category name as value
            $db_obj = new DB();
            $this->category_list = array();
            $query = "SELECT category_id,name FROM category";
            $resultset = $db_obj->query($query);
            while ($row = array_pop($resultset))
            {
                $this->category_list[$row['category_id']] = $row['name'];
            }
        }

        function get_article_category_array($id)
        {
            $db_obj = new DB();
            $this->cat_match = array();
            $query = "SELECT catid FROM catlink WHERE id=$id GROUP BY catid";
            $resultset = $db_obj->query($query);

            while ($row = array_pop($resultset))
            {
                $this->cat_match[] = $row['catid'];
            }

        }

// function adde to display the list of category names an artile belogns to -- blicero

        function get_article_catname_array($id)
        {
            $db_obj = new DB();
            $this->cat_name_match = array();
            $query = "SELECT catlink.catid,category.shortname FROM catlink LEFT JOIN category ON catlink.catid = category.category_id WHERE id = $id GROUP BY catlink.catid";
            $resultset = $db_obj->query($query);
            $this->cat_name_match_printable='$GLOBALS["page_categories"]=array(';
	    if (is_array($resultset)) {
                while ($row = array_pop($resultset))
                {
                    $this->cat_name_match[] = $row['shortname'];
                    $this->cat_name_match_printable .= 'array('.$row['catid'].",'".$row['shortname']."'),";
                }
                $this->cat_name_match_printable .= ');';
                $this->cat_name_match_printable = str_replace(',);',');',$this->cat_name_match_printable);
            }
        }

    function get_list()
    {
        //returns a list of all fetaures in teh sysetm.
        $db_obj = new DB();
	$category_list_query="SELECT * FROM category order by order_num desc";
        $result = $db_obj->query($category_list_query);
        return $result;
    }
// function added to retunr only categories of a single class -- blicero 22 may 2003 
   
    function get_class_list($class)
    {
        //returns a list of all fetaures in teh sysetm.
        $db_obj = new DB();
	$category_list_query="SELECT * FROM category where catclass = '$class' order by order_num desc";
        $result = $db_obj->query($category_list_query);
        return $result;
   }  

// function added to retunr only active categories -- blicero 22 may 2003 
    function get_active_list()
    {
        //returns a list of all fetaures in teh sysetm.
        $db_obj = new DB();
	//$category_list_query="SELECT * FROM category where catclass = 't' or  catclass='l' or catclass = 'i' order by order_num desc";
	$category_list_query = "select * from category where catclass != 'm' and catclass != 'h' order by order_num desc ";
        $result = $db_obj->query($category_list_query);
        return $result;
   } 

    function cache_left_catlisting() 
	{
	
	$tr=new \Translate();
	$tr->create_translate_table('category');
	$catfeed=$GLOBALS['visible_catclass'];
 	// cycle all the class considered visibles
       while ($char < strlen($catfeed))
        {
	// create teh category listing beloging to that catclass
	// if the var is set to m produce a single listing of categories (default behaviour)
	if ($catfeed=="m") {
	$catlist=$this->get_active_list();
	}else{
	$catlist=$this->get_class_list($catfeed[$char]);
	}
	$catindex = new \FastTemplate(SF_TEMPLATE_PATH."/");
        $catindex->clear_all();
        $catindex->define(array('page' => "left_catlisting.tpl"));
        $catindex_param=array();
        // commented because they should not be needed :) 
	//if (is_array($catlist)) {
	while ($catrow=array_pop($catlist)) {
	$catindex_param["TPL_FTRADDRESS"]=SF_FEATURE_URL."/".$catrow['shortname']."/";
        $catindex_param["TPL_FTRNAME"]=$catrow['name'];
        $catindex->assign($catindex_param);
        $catindex->parse('CONTENT',"page");
        $htmlcode.=$catindex->fetch("CONTENT");
		}
	/*}else{
        $catindex_param["TPL_FTRADDRESS"]=SF_FEATURE_URL."/".$catlist['shortname']."/";
        $catindex_param["TPL_FTRNAME"]=$catlist['name'];
        $catindex->assign($catindex_param);
        $catindex->parse('CONTENT',"page");
        $htmlcode.=$catindex->fetch("CONTENT");
	}*/
	
	// create the catclass block
	$catblock=new \FastTemplate(SF_TEMPLATE_PATH."/");
	$catblock->clear_all();
	$catblock->define(array('page'=>"left_catlist.tpl"));
	$catblock_param["TPL_CATCLASS"]= "<?php echo(\$sftr->trans('".$catfeed[$char]."_category_left')) ;?>";
	$catblock_param["TPL_CATLISTING"]=$htmlcode;
        $catblock->assign($catblock_param);
	$catblock->parse('CONTENT',"page");
	// we clean the previous cycle before starting again :)
	$htmlcode="";
	$htmlfinal.=$catblock->fetch("CONTENT");
        $char++;
        }
// save all the blocks in one file 	
	$catdirindex=SF_CACHE_PATH."/left_catlisting.inc";
	$this->cache_file($htmlfinal,$catdirindex);

	}	
    
	
	function get_thematic_assoc_list()
    {
        // returns a list of all categories w/newswires for make_select_form()
        // the following two methods can probably be expired
        $db_obj = new DB();

	//inserted translation into a embedded select a category
	$tr = new \Translate;	
        $return_array = array();
	$query = "SELECT * FROM category WHERE newswire != 'n' and catclass='t' ORDER BY order_num DESC;";
        $result = $db_obj->query($query);
	$return_array[0] = $tr->trans('select_a_thematic_category');

        while ($row = array_pop($result))
        {
            $key = $row['category_id'];
	    $return_array[$key] = $row['name'];
        }
        return $return_array;
    }

    function cache_thematic_assoc_list()
    {
        // caches an array of all thematic/internal categories with a newswire for a select form.
        // this get called when categories change. so we don't have to select them each time someones hits publish.php
        $db_obj = new DB;
        $query = "select category_id, name from category where newswire != 'n' and catclass = 't' order by order_num desc";
        $result = $db_obj->query($query);

        $file = "<?php\n\n";
        $file .= "\$cattema_options[\"0\"]  =  \$this->tr->trans('select_a_thematic_category');\n";

        while($row = array_pop($result))
        {
	        $file .= "\$cattema_options[\"".$row['category_id']."\"]  =  \"".$row['name']."\";\n";
        }
        $file .= "\n?>";
        $this->cache_file($file, SF_CACHE_PATH.'/cattema_options.inc');
    }

    function get_local_assoc_list()
    {
        // returns a list of all categories w/newswires for make_select_form()
        // the following two methods can probably be expired
        $db_obj = new DB();

	//inserted translation into a embedded select a category
	$tr = new \Translate;	
        $return_array = array();
	$query = "SELECT * FROM category WHERE newswire != 'n' and catclass='l' ORDER BY order_num DESC;";
        $result = $db_obj->query($query);
	$return_array[0] = $tr->trans('select_a_local_category');

        while ($row = array_pop($result))
        {
            $key = $row['category_id'];
	    $return_array[$key] = $row['name'];
        }
        return $return_array;
    }

    function cache_local_assoc_list()
    {
       $db_obj = new DB;
        $query = "select category_id, name from category where newswire != 'n' and catclass = 'l' order by order_num desc";
        $result = $db_obj->query($query);

        $file = "<?php\n\n";
        $file .= "\$catlocali_options[\"0\"]  =  \$this->tr->trans('select_a_local_category');\n";

        while($row = array_pop($result))
        {
            $file .= "\$catlocali_options[\"".$row['category_id']."\"]  =  \"".$row['name']."\";\n";
        }

        $file .= "\n?>";
        $this->cache_file($file, SF_CACHE_PATH.'/catlocali_options.inc');
    }

    function get_newswire_assoc_list()
    {
        // returns a list of all categories w/newswires for make_select_form()
        // the following two methods can probably be expired
        $db_obj = new DB();

	//inserted translation into a embedded select a category
	$tr = new \Translate;	
        $return_array = array();

        $query = "SELECT * FROM category WHERE newswire != 'n' ORDER BY order_num DESC";
        $result = $db_obj->query($query);
        while ($row = array_pop($result))
        {
            $key = $row['category_id'];

            if ($key == $GLOBALS['config_defcategory'])
            {
                $return_array[0] = $tr->trans('select_a_category');
            } else
            {
                $return_array[$key] = $row['name'];
            }
        }
        return $return_array;
    }
	
    function cache_newswire_assoc_list()
    {
        $db_obj = new DB;
        $query = "select category_id, name from category where newswire != 'n' and category_id != '".$GLOBALS['config_defcategory']."' order by order_num desc";
        $result = $db_obj->query($query);

        $file = "<?php\n\n";
        $file .= "\$cat_options[\"0\"]  =  \$this->tr->trans('select_a_category');\n";

        while($row = array_pop($result))
        {
            $file .= "\$cat_options[\"".$row['category_id']."\"]  =  \"".$row['name']."\";\n";
        }

        $file .= "\n?>";
        $this->cache_file($file, SF_CACHE_PATH.'/cat_options.inc');
    }

    function cache_internal_assoc_list()
    {
	$db_obj = new DB;
	$query = "select category_id, name from category where newswire !='n' and catclass = 'i' order by order_num desc";
	$result = $db_obj->query($query);

	$file = "<?php\n\n";
	$file .= "\$catinternal_options['0'] = \$GLOBALS['dict']['select_a_internal_category'];\n";

	while($row = array_pop($result))
	{
	    $file .= "\$catinternal_options[\"".$row['category_id']."\"]  =  \"".$row['name']."\";\n";
	}
	$file .= "\n?>";
	$this->cache_file($file, SF_CACHE_PATH.'/catinternal_options.inc');
    }

    function cache_hidden_assoc_list()
    {
        $db_obj = new DB;
        $query = "select category_id, name from category where newswire !='n' and catclass = 'h' order by order_num desc";
        $result = $db_obj->query($query);

        $file = "<?php\n\n";
        $file .= "\$cathidden_options['0'] = \$GLOBALS['dict']['select_a_hidden_category'];\n";

        while($row = array_pop($result))
        {
            $file .= "\$cathidden_options[\"".$row['category_id']."\"]  =  \"".$row['name']."\";\n";
        }
        $file .= "\n?>";
        $this->cache_file($file, SF_CACHE_PATH.'/cathidden_options.inc');
    }

    function cache_project_assoc_list()
    {
        $db_obj = new DB;
        $query = "select category_id, name from category where newswire !='n' and catclass = 'p' order by order_num desc";
        $result = $db_obj->query($query);

        $file = "<?php\n\n";
        $file .= "\$catproject_options['0'] = \$GLOBALS['dict']['select_a_project_category'];\n";

        while($row = array_pop($result))
        {
            $file .= "\$catproject_options[\"".$row['category_id']."\"]  =  \"".$row['name']."\";\n";
        }
        $file .= "\n?>";
        $this->cache_file($file, SF_CACHE_PATH.'/catproject_options.inc');
    }

    function cache_event_assoc_list()
    {
        $db_obj = new DB;
        $query = "select category_id, name from category where newswire !='n' and catclass = 'e' order by order_num desc";
        $result = $db_obj->query($query);

        $file = "<?php\n\n";
        $file .= "\$catevent_options['0'] = \$GLOBALS['dict']['select_a_event_category'];\n";

        while($row = array_pop($result))
        {
            $file .= "\$catevent_options[\"".$row['category_id']."\"]  =  \"".$row['name']."\";\n";
        }
        $file .= "\n?>";
        $this->cache_file($file, SF_CACHE_PATH.'/catevent_options.inc');
    }

    function cache_other_assoc_list()
    {
        $db_obj = new DB;
        $query = "select category_id, name from category where newswire !='n' and catclass = 'o' order by order_num desc";
        $result = $db_obj->query($query);

        $file = "<?php\n\n";
        $file .= "\$catother_options['0'] = \$GLOBALS['dict']['select_a_other_category'];\n";

        while($row = array_pop($result))
        {
            $file .= "\$catother_options[\"".$row['category_id']."\"]  =  \"".$row['name']."\";\n";
        }
        $file .= "\n?>";
        $this->cache_file($file, SF_CACHE_PATH.'/catother_options.inc');
    }


    function cache_publish_arrays()
    {
        // this function will decide which arrays need to be saved based on the value of $GLOBALS['multicat']
        if($GLOBALS['multicat'] == 1)
        {
            $this->cache_thematic_assoc_list();
            $this->cache_local_assoc_list();
	    $this->cache_hidden_assoc_list();
	    $this->cache_event_assoc_list();
	    $this->cache_project_assoc_list();
	    $this->cache_other_assoc_list();
	    $this->cache_internal_assoc_list();
        }
	// we need this one for sure. it's used in /news/
        $this->cache_newswire_assoc_list();
	// this one isn't for publish but for article2feature & copyfeature stuff.
	$this->cache_feature_assoc_list();        
    }

    function get_feature_assoc_list()
    {
        // returns a list of all categories w/central column for make_select_form()
        // the following two methods can probably be expired
        $db_obj = new DB();
        $return_array = array();
	// added the where center='t' -- blicero 20 may 2003
        $query = "SELECT * FROM category WHERE center='t' ORDER BY order_num DESC";
        $result = $db_obj->query($query);
        while ($row = array_pop($result))
        {
		$key = $row['category_id'];
        	$return_array[$key] = $row['name'];
        }
        return $return_array;
    }

    function cache_feature_assoc_list()
    {
			// fetches a list of all categories w</central column for make_select_form()
			// the difference with get_feature_assoc_list() is that it's saves the return_array
			// on disk (so we don't have to select it each time).

			$db_obj = new DB ;
			$query = "select * from category where center = 't' order by order_num desc";
			$result = $db_obj->query($query);
			$file = "<?php\n\n";
			while($row = array_pop($result))
			{
				$file .= "\$cat_options[\"".$row['category_id']."\"] = \"".$row["name"]."\";\n";
			}
			$file .= "\n?>";
			$this->cache_file($file, SF_CACHE_PATH.'/feature_options.inc');
    }

    function get_newswire_list()
    {
        //returns a list of all features with newswires in the system
        $db_obj = new DB();
        $category_list_query="SELECT * FROM category WHERE newswire != 'n' order by order_num desc";
        $result=$db_obj->query($category_list_query);
        return $result;
    }

    function render_select_list($id=0)
    {
        //generates html for select list

        $tr = new \Translate;
        $result = $this->get_newswire_list();
        $result_html = "<select name=\"category\">\n";
        $result_html .= "<option value=\"\">" . $tr->trans('select_a_category') . "</option>\n";
        while ($row=array_pop($result))
        {
            if ($default_category != $row['category_id'])
            {
                $result_html .= "<option value=\"" . $row['category_id'] . "\" <" . "?php ";
                $result_html .= "if ($" . "category==" . $row['category_id'] . ") echo \"selected\" ?" . ">>";
                $result_html .= $row['name'];
                $result_html .= "</option>\n";
            }
        }
        $result_html .= "</select>\n";
        return $result_html;
    }

    function add_article_to_category($article_id, $category_id)
    {
        $db_obj = new DB();
        $query = "INSERT INTO catlink (catid,id) VALUES (" . $category_id . "," . $article_id . ")";
        $result = $db_obj->execute($query);
        return 1;
    }

    function add($categoryfields)
    {
        //Adds a feature given the fetures fields in a dictionary object.
        $db_obj = new DB();
        if (strlen($categoryfields['shortname'])!=0 && strlen($categoryfields['name'])!=0 && is_numeric($categoryfields['order_num']))
        {
            
		// added a regexp to avoid spaces in category shortname
	    $categoryfields['shortname']=preg_replace("/ /","_",$categoryfields['shortname']);
	    if (!$categoryfields['summarylength']) $categoryfields['summarylength']=10;
            if (!$categoryfields['default_feature_template_name']) $categoryfields['default_feature_template_name']="html_only";
            if (!$categoryfields['template_name']) $categoryfields['template_name']="feature_list";
            if (!$categoryfields['parentid']) $categoryfields['parentid']=1;
            if (!$categoryfields['newswire']) $categoryfields['newswire']="n";
            if (!$categoryfields['center']) $categoryfields['center']=f;
	// added catclass column -- blicero
            if (!$categoryfields['catclass']) $categoryfields['catclass']=h;
            $category_add_query="INSERT INTO category (";
            $category_add_query=$category_add_query."name, template_name, order_num, default_feature_template_name";
            $category_add_query.=",shortname,summarylength,parentid,newswire,center,catclass,description";
            $category_add_query.=") VALUES ('";
            $category_add_query.=$categoryfields['name']."', '";
            $category_add_query.=$categoryfields['template_name']."', ";
            $category_add_query.=$categoryfields['order_num'].", '";
            $category_add_query.=$categoryfields['default_feature_template_name']."', '";
            $category_add_query.=$categoryfields['shortname']."',";
            $category_add_query.=$categoryfields['summarylength'].",";
            $category_add_query.=$categoryfields['parentid'].",'";
            $category_add_query.=$categoryfields['newswire']."','";
            $category_add_query.=$categoryfields['center']."','";
            $category_add_query.=$categoryfields['catclass']."','";
            $category_add_query.=$categoryfields['description']."')";
            $error_num=$db_obj->execute($category_add_query);
            #force to no error since not yet checking db for error
            $error_num=0;
	// add on to create the dir for features
		if (($categoryfields['catclass'] != 'h') && ($categoryfields['catclass'] != 'm')) {
		$this->add_catdir($categoryfields['shortname']);	
		$this->cache_category_dir_index($categoryfields);
		$this->cache_left_catlisting();
        $this->cache_publish_arrays();

		}
	
        // add to touch the files in SF_CACHE_PATH. avoids gettings warnings when you visit just seup categories.
	if($categoryfields['newswire'] !== "n")
	{
		touch(SF_CACHE_PATH.'/'.$categoryfields['shortname'].'_summaries.html');
	}
	if($categoryfields['center'] == 't')
	{
		touch(SF_CACHE_PATH.'/center_column_'.$categoryfields['shortname'].'.html');
	}
	
	}else
        {
            $error_num=1;
        }
        return $error_num;
    }

// function to create a category directory  -- blicero 22 may 2003

	function add_catdir($categoryname) {
		if (!is_dir(SF_FEATURE_PATH."/".$categoryname)){
                	mkdir(SF_FEATURE_PATH."/".$categoryname,0775);
		}
		return 1;
	}

// function to cache a category directory index -- blicero 22 may 2003

	function cache_category_dir_index($categoryfields) {
		
        $catindex = new \FastTemplate(SF_TEMPLATE_PATH."/");
        $catindex->clear_all();
        $catindex->define(array('page' => "index_category.tpl"));
        $catindex_param=array();
        $catindex_param["TPL_CATID"]=$categoryfields['category_id'];
        $catindex_param["TPL_FEATURE_NAME"]=$categoryfields['shortname'];
        $catindex_param["TPL_TITLE"]=$categoryfields['name'];
        $catindex->assign($catindex_param);
        $catindex->parse('CONTENT',"page");
        $htmlcode=$catindex->fetch("CONTENT");
	$catdirindex=SF_FEATURE_PATH."/".$categoryfields['shortname']."/index.php";
	$this->cache_file($htmlcode,$catdirindex);
	
	}
    function update($categoryfields)
    {
        //Updates a feature given the fetaures fields in a dictionary object.
        $db_obj = new DB();
        if (strlen($categoryfields['category_id'])!=0 && strlen($categoryfields['shortname'])!=0 && strlen($categoryfields['name'])!=0 && is_numeric($categoryfields['order_num']))
        {
		// added a regexp to avoid spaces in category shortname
	    $categoryfields['shortname']=preg_replace("/ /","_",$categoryfields['shortname']);
            if (!$categoryfields['summarylength']) $categoryfields['summarylength']=10;
            if (!$categoryfields['default_feature_template_name']) $categoryfields['default_feature_template_name']="html_only";
            if (!$categoryfields['template_name']) $categoryfields['template_name']="feature_list";
            if (!$categoryfields['parentid']) $categoryfields['parentid']=1;
            if (!$categoryfields['newswire']) $categoryfields['newswire']="n";
            if (!$categoryfields['center']) $categoryfields['center']=f;
            if (!$categoryfields['catclass']) $categoryfields['catclass']=h;
            $category_update_query="UPDATE category SET ";
            $category_update_query.="name='";
            $category_update_query.=$categoryfields['name'];
            $category_update_query.="', template_name='".$categoryfields['template_name'];
            $category_update_query.="', default_feature_template_name='".$categoryfields['default_feature_template_name'];
            $category_update_query.="', order_num=".$categoryfields['order_num'];
            $category_update_query.=", shortname='".$categoryfields['shortname'];
            $category_update_query.="', summarylength=".$categoryfields['summarylength'];
            $category_update_query.=", parentid=".$categoryfields['parentid'];
            $category_update_query.=", newswire='".$categoryfields['newswire'];
            $category_update_query.="', center='".$categoryfields['center'];
	// added catclass column :) -- blicero
            $category_update_query.="', catclass='".$categoryfields['catclass'];
            $category_update_query.="', description='".$categoryfields['description'];
            $category_update_query.="' WHERE category_id=".$categoryfields['category_id'];
            #echo $category_update_query;
            $error_num=$db_obj->execute($category_update_query);
            #force to no error since not yet checking db for error
            $error_num=0;
	// add on to create the dir for features
		if (($categoryfields['catclass'] != 'h') && ($categoryfields['catclass'] != 'm')) {
		$this->add_catdir($categoryfields['shortname']);	
		//set this so that only if explicitly selected the update will result in a recaching of the category directory file -- blicero
		if ($_POST['recache']=="true") { $this->cache_category_dir_index($categoryfields); }
		//$this->cache_category_dir_index($categoryfields);
		$this->cache_left_catlisting();
        $this->cache_publish_arrays();

		
		}
	} else
        {
            $error_num=1;
        }
        return $error_num;
    }
    
    function delete($category_id)
    {
        //Deletes a feature given the feature's id
        $db_obj = new DB();
        if (strlen($category_id)!=0)
        {
            $category_delete_query="DELETE FROM category WHERE category_id=";
            $category_delete_query=$category_delete_query.$category_id;
            echo $category_delete_query;
            $error_num=$db_obj->execute($category_delete_query);
            $error_num=0;    
        } else
        {
            $error_num=1;
        }
        return $error_num;
    }


    function reorder($posted_fields)
    {
        //Reorders the list of features. This will mainly be useful
        //when the main pages list of features is no longer hardcoded.
        $db_obj = new DB();
        $query1="UPDATE category set order_num=";
        $query2=" WHERE category_id=";
        $querykeys=array_keys($posted_fields);
        $nextkey=array_pop($querykeys);
        while (strlen($nextkey)!=0)
        {
            if (preg_match("/category_order/", $nextkey))
            {
                $query=$query1.$posted_fields["$nextkey"];
                $query=$query.$query2.substr($nextkey,14,strlen($nextkey)-14);
                $db_obj->execute($query);
            }
            $nextkey=array_pop($querykeys);
        }
// added by blicero to refresh feature listing -- blicero 22 may 2003		
	$this->cache_left_catlisting();
    $this->cache_publish_arrays();

	
        return 0;
    }

    function render_feature_list($category_id)
    {
        //Renders the center column portion of a feature
        $db_obj = new DB();

        $template_obj = new \FastTemplate(SF_TEMPLATE_PATH);
        $feature_obj = new \SFACTIVE\Feature();
        $temp_string="";
        $feature_list=$feature_obj->get_list($category_id, "c");
        $categoryfields=$this->get_category_fields($category_id);
        $default_features_template_name=$categoryfields['default_features_template_name'];
        while ($nextfeature=array_pop($feature_list))
        {
            $temp_string=$temp_string.$feature_obj->render_feature($nextfeature['feature_version_id']);
        }
        $defaults = (array(    'ROWS'     => "$temp_string"));
        
        if (strlen($categoryfields['template_name'])>0)
        {
            $template_obj->define(array('feature_list' => $categoryfields['template_name'].".tpl"));
            $template_obj->assign($defaults);
            $template_obj->parse('CONTENT', "feature_list");
            $result_html = $template_obj->fetch("CONTENT");
        } else
        {
	    $tr = new \Translate ;
	    $tr->create_translate_table('category');
            $result_html= $tr->trans('no_feature_template_defined') ; 
        }
        return $result_html;
    }

    function render_feature_page($category_id, $page)
    {
        //Renders a page for the paged version of stories for a given feature
        $db_obj = new DB();
        $feature_obj = new Feature;
        $template_obj = new \FastTemplate(SF_TEMPLATE_PATH);

        $temp_string = "";
        $feature_list = $feature_obj->get_list($category_id, "a");
        $categoryfields = $this->get_category_fields($category_id);
        $default_features_template_name = $categoryfields['default_features_template_name'];

        $i = 0;

        if (strlen(trim($default_feature_template_name)) < 1)
        {
            $default_feature_template_name = "html_row";
        }

        while (($nextfeature = array_pop($feature_list)) && $i < $page * 5)
        {
            if (($i / 5) >= $page - 1)
            {
                $temp_string .= $feature_obj->render_feature($nextfeature['feature_version_id']);
            }
            $i = $i+1;
        }
        
        $defaults = (array(   'ROWS'     => "$temp_string"));
        
        if (strlen(trim($categoryfields['template_name'])) < 1)
        {
            $categoryfields['template_name'] = "feature_list";
        }

        $template_obj->define(array('feature_list' => $categoryfields['template_name'].".tpl"));
        $template_obj->assign($defaults);
        $template_obj->parse('CONTENT', "feature_list");
        $result_html = $template_obj->fetch("CONTENT");
        return $result_html;
    }

    function cache_archives_for_week($category_id,$date){

	$categoryfields = $this->get_category_fields($category_id);

	$html_for_week=$this->render_archives_for_week($category_id,$date);
	
	$date->find_start_of_week();
	$month=$date->get_month();
	$day=$date->get_day();
	$year=$date->get_year();

        // $end_php_string  = "\n<?php include($"."local_include_path.\"/content-crumblink.inc\");";
        // $end_php_string .= "include($"."local_include_path.\"/content-footer.inc\")?" . ">";

        if ($category_id != $GLOBALS['config_defcategory']){
        	$category_path=$categoryfields['shortname']."/";
        }else{
        	$category_path="";
        }

        if ($category_id != $GLOBALS['config_defcategory']){
        	$category_string = "?category=".$categoryfields['shortname'];
        }else{
        	$category_string="";
        }

        $display_string  = $html_for_week;

        if ($category_id != $GLOBALS['config_defcategory']){
                        $category_path = $categoryfields['shortname'] . "/";

                        if (!file_exists($GLOBALS['archive_cache_path']."/".$category_path))
                        {
                            mkdir($GLOBALS['archive_cache_path']."/".$category_path,0775);
                        }
                }

                $filename = $GLOBALS['archive_cache_path']."/".$category_path.$year."_".$month."_".$day.".php";

                $fffp = fopen($filename, "w");
		//echo "writing to $filename";
                fwrite($fffp, $display_string, strlen($display_string));
                fclose($fffp);  		
    }

    function render_archives_for_week($category_id,$date)
    {
        //Renders an archive week of stories for a given feature
        $db_obj = new DB();
        $feature_obj = new Feature;
        $template_obj = new \FastTemplate(SF_TEMPLATE_PATH);
        global $items_existed_for_week;

        $items_existed_for_week=false;
        $temp_string="";
        $feature_list=$feature_obj->get_archives_for_week($category_id, $date);
        $categoryfields=$this->get_category_fields($category_id);
        $default_features_template_name=$categoryfields['default_features_template_name'];
        while ($nextfeature=array_pop($feature_list))
        {
            $items_existed_for_week=true;
//          $temp_string.=$nextfeature["modification_date_year"]."-".$nextfeature["modification_date_month"]."-".$nextfeature["modification_date_day"]." ";
//          $temp_string.=$nextfeature["modification_date_hour"].":".$nextfeature["modification_date_minute"]."<br>";
	    $temp_string.=$feature_obj->render_feature($nextfeature['feature_version_id']);
        }
        $defaults = (array(    'ROWS'     => "$temp_string"));

        if (strlen($categoryfields['template_name'])>0)
        {
            $template_obj->define(array('feature_list' => $categoryfields['template_name'].".tpl"));
            $template_obj->assign($defaults);
            $template_obj->parse('CONTENT', "feature_list");
            $result_html = $template_obj->fetch("CONTENT");
        } else
        {
	    $tr = new \Translate ;
	    $tr->create_translate_table('category');
            $result_html= $tr->trans('no_category_template_defined') ; 
        }
        return $result_html;
    }

    function render_single_feature_archive($feature_id)
    {
        //render a single feature in the archives/cache/single/ dir
        $db_obj = new DB();
        //global $feature_obj;
	$feature_obj=new Feature();
        //global $template_obj;
	$template_obj=new \FastTemplate(SF_TEMPLATE_PATH);
		$tr = new \Translate;

        $feature_query = "SELECT feature_version_id,category_id,title2 from feature where feature_id='".$feature_id."' and is_current_version='1'";
        $result_feature = $db_obj->query($feature_query);
        $result_feature = array_pop($result_feature);
        if (!is_array($result_feature))
        {
            $msg= $tr->trans('no_items_for_id')." ".$feature_id."<br>";
        } else
        {
            $feature_version_id = $result_feature['feature_version_id'];
            $categoryfields=$this->get_category_fields($result_feature['category_id']);
            //we create the single featrue text with minimum navigation bar
            $body=$feature_obj->render_feature($feature_version_id);
            $result_feature['title2'] = preg_replace("/\"/","&quot;",$result_feature['title2']);
			$display_string=$body;
        //check if the single dir exists
         if (!file_exists($GLOBALS['archive_cache_path']."/single")) {
                                         mkdir($GLOBALS['archive_cache_path']."/single",0775); } 
                     
        //we dump the feature in a cache file 
        
        $filename=$GLOBALS['archive_cache_path']."/single/".$feature_id.".php";
        $fffp=fopen($filename, "w");
        fwrite($fffp, $display_string, strlen($display_string));
        fclose($fffp);
        $msg= $tr->trans('writing')." ".$filename."<br>";
        }
        return $msg;
    }

    function get_syndicated_features($summary, $category)
    {
        $feature_length = $GLOBALS['feature_length'];
        if($summary != "") { $summary = "s.summary as summary, " ; }
        if($category != "") { $category = "and c.name='".$category."' "; }
        $db_obj= new DB;
        $fq = "select s.feature_id as feature_id, s.category_id as category_id, ".$summary;
        $fq .= "DATE_FORMAT(s.modification_date,'%Y-%m-%d %H:%i:%s') as date, ";
        $fq .= "s.title1 as title1, s.title2 as title2, c.name as name, c.shortname as shortname, s.image as image, s.language_id as language_id from feature s,category c ";
        $fq .= "where s.status='c' and s.category_id=c.category_id ".$category;
        $fq .= "and s.is_current_version='1' order by s.creation_date desc limit $feature_length";
        $featureset = $db_obj->query($fq);
        return $featureset;
    }

    function make_features_rdf($summary, $category)
    {
	$GLOBALS['print_rss'] = "" ;
	$lang_obj = new \language ;
	$featureset=$this->get_syndicated_features($summary, $category);
	if($category != "") { $category = $featureset['0']['shortname']."_" ;}
	$rows = sizeof($featureset);
	$webroot_url = SF_ROOT_URL;
	$site_nick = $GLOBALS['site_nick'];
	$site_name = $GLOBALS['site_name'];
	$feature_file = $GLOBALS['feature_file'];
	$xml_logo = $GLOBALS['xml_logo'];
	include_once(SF_SHARED_PATH . '/classes/rss10.inc');
	$rss=new \RSSWriter($webroot_url, $site_nick, $site_name, $webroot_url."/syn/".$category.$feature_file, array("dc:publisher" => $site_nick, "dc:creator" => $site_nick, "dc:language" => $GLOBALS['site_lang']));
	$rss->setImage($xml_logo, $site_nick);
	foreach (array_reverse($featureset) as $row)
	{
            $link = SF_ARCHIVE_URL."/archive_by_id.php?id=" . $row['feature_id'];
	    $link .= "&amp;category_id=".$row['category_id'];
	    $title1 = trim(utf8_encode(htmlspecialchars($row['title1'])));
	    $title2 = trim(utf8_encode(htmlspecialchars($row['title2'])));
	    $cat_name = trim(utf8_encode(htmlspecialchars($row['name'])));
            $date = gmdate('Y-m-d\TH:i:s\Z',strtotime($row['date']));
	    $language = trim(utf8_encode($lang_obj->get_language_code($row['language_id'])));
	    $rss->addItem($link, $title1, array("description" => $title2, "dc:date" => $date, "dc:subject" => $cat_name, "dc:creator" => $site_nick, "dc:language" => $language));
        }
        $print=$rss->serialize();
        $print=str_replace(array("&amp;#","&amp;amp;","&amp;gt;","&amp;lt;","&amp;quot;"), array("&#","&amp;","&gt;","&lt;","&quot;"), $print);
        $fffp = fopen(SF_WEB_PATH."/syn/".$category.$feature_file, "w");
        fwrite($fffp, $print, strlen($print));
	fclose($fffp);
    }


    function make_features_long_rdf($summary, $category)
    {
        $GLOBALS['print_rss'] = "" ;
	    $lang_obj = new \language ;
        $featureset=$this->get_syndicated_features($summary, $category);
        if($category != "") { $category = $featureset['0']['shortname']."_" ;}
        if($GLOBALS['lang'] == "hr") { $char = "utf-8" ; }
        else { $char = "cp1252";}
        $rows = sizeof($featureset);
        $webroot_url = SF_ROOT_URL;
        $site_nick = $GLOBALS['site_nick'];
        $site_name = $GLOBALS['site_name'];
        $feature_file = $GLOBALS['feature_file_long'] ;
        $xml_logo = $GLOBALS['xml_logo'];
        include_once(SF_SHARED_PATH . '/classes/rss10.inc');
        $rss_long=new \RSSWriter($webroot_url, $site_nick, $site_name, $webroot_url."/syn/".$category.$feature_file, array("dc:publisher" => $site_nick, "dc:creator" => $site_nick, "dc:language" => $GLOBALS['site_lang']));
        $rss_long->useModule("content", "http://purl.org/rss/1.0/modules/content/");
	$rss_long->useModule("dcterms", "http://purl.org/dc/terms/");
        $rss_long->setImage($xml_logo, $site_nick);
        foreach (array_reverse($featureset) as $row)
        {
            $link = SF_ARCHIVE_URL."/archive_by_id.php?id=" . $row['feature_id'];
            $link .= "&amp;category_id=".$row['category_id'];
            if(preg_match('#^https*://#', $row['image'])==0)
            {
                $file = $row['image'] ;
            }elseif(strpos('/', $row['image'])==0){
                $file = SF_ROOT_URL.$row['image'] ;
            }else {
                $file = '';
            }
            $title1 = trim(utf8_encode(htmlspecialchars($row['title1'])));
            $title2 = trim(utf8_encode(htmlspecialchars($row['title2'])));
            $cat_name = trim(utf8_encode(htmlspecialchars($row['name'])));
	    $language = trim(utf8_encode($lang_obj->get_language_code($row['language_id'])));
            $summary = utf8_encode(str_replace(array('&lt;','&gt;','&amp;','€','‘','’','“','”'), array('<','>','&','&euro;','&lsquo;','&rsquo;','&ldquo;','&rdquo;'),htmlentities(stripslashes($row['summary']), ENT_NOQUOTES, $char)));
            $date = gmdate('Y-m-d\TH:i:s\Z',strtotime($row['date']));
            # original -- $rss_long->addItem($link, $title1, array("description" => $title2, "dc:date" => $date, "dc:subject" => $cat_name, "dc:creator" => $site_nick, "dc:language" => $language, "content:encoded" => $summary, "dcterms:hasPart" => $file));
			# altering feed so the 'description' now holds the content - johnk
            $rss_long->addItem($link, $title1, array("description" => $summary, "dc:date" => $date, "dc:subject" => $cat_name, "dc:creator" => $site_nick, "dc:language" => $language, "content:encoded" => $summary, "dcterms:hasPart" => $file));
        }
        $print_long=$rss_long->serialize();
        $print_long=str_replace(array("&amp;#","&amp;amp;","&amp;gt;","&amp;lt;","&amp;quot;"),array("&#","&amp;","&gt;","&lt;","&quot;"),$print_long);
        $fffp = fopen(SF_WEB_PATH."/syn/".$category.$feature_file, "w");
        fwrite($fffp, $print_long, strlen($print_long));
        fclose($fffp);
    }


    function get_center_list()
    {
        //returns a list of all features with center columns in the system
        $db_obj = new DB();
        // added control on catclass -- blicero
	//$category_list_query="select * from category where center = 't' order by order_num desc";
        $category_list_query="select * from category where center = 't' and catclass!='h' order by order_num desc";
        $result=$db_obj->query($category_list_query);
        return $result;
    }

    function get_max_id(){
	$db_obj= new DB;
    	$return_val=0;
	$result=$db_obj->query("Select max(category_id) as id from Category");
	$row=array_pop($result);
	$t_return_val=$row["id"];
	if (strlen($t_return_val)>0){
		$return_val=$t_return_val;
	}	
	return $return_val;
    }
}

