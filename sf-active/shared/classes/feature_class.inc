<?php

//include_once('knowmore_class.inc');

class Feature 
{
    //The feature class represents a blurb in the center column of a feature
    //The naming convenstion now are off so what is refered to as Feature in
    //code is story or blurb in teh html. And what is refered to as Category in the 
    //code is always refered to as fetaure in the HTML.
    function get_latest_feature($feature_id){ 
	global $db_obj;
	$db_obj = new DB;
	$hist_list=$this->get_history_list($feature_id);
	$hist_item=array_pop($hist_list);
	$new_feature= new Feature;
	$feature_fields=$new_feature->get_feature_fields($hist_item["feature_version_id"]);
	return $feature_fields;
    }

    function get_feature_fields($feature_version_id)
    {
        //returns a dictionary object containing all columns for a story
        //giev the story's id
        $db_obj = new DB;

        if (strlen($feature_version_id) != 0)
        {

            $query  = "select feature_version_id, feature_id, display_date, modifier_id, ";
            $query .= "YEAR(creation_date) as creation_date_year, ";
            $query .= "MONTH(creation_date) as creation_date_month, ";
            $query .= "DAYOFMONTH(creation_date) as creation_date_day, ";
            $query .= "YEAR(modification_date) as modification_date_year, ";
            $query .= "MONTH(modification_date) as modification_date_month, ";
            $query .= "DAYOFMONTH(modification_date) as modification_date_day, ";
            $query .= "date_format(modification_date, '%W %M %d, %Y at %h:%i %p') as mod_date, ";
            $query .= "date_format(creation_date, '%W %M %d, %Y at %h:%i %p') as creat_date, ";
            $query .= "is_current_version, status, tag, image,image_link, version_num, ";
            $query .= "title1, title2, summary, template_name, order_num, category_id, language_id ";
            $query .= "from feature where feature_version_id=";
            $query .= $feature_version_id;

            $resultset = $db_obj->query($query);
            $feature = array_pop($resultset);

            if (strlen($feature['modifier_id']) > 0)
            {
                $query = "select username from user where user_id=" . $feature['modifier_id'];
                $resultset = $db_obj->query($query);
                $userobj = array_pop($resultset);
                $username = $userobj['username'];
                $feature['modifier_name'] = $username;
            }
            return $feature;
        } else
        {
            return 1;
        }
    }

    function get_list($category_id, $status)
    {
        //Returns a list of feature dictionary objects given a feature id (category id in the code)
        //and a status
		$get = array_merge(array( 'regen' => null), $_GET);

        $db_obj = new DB;

        $feature_list_query = "select feature_id, feature_version_id, status, ";
        $feature_list_query .= "title1, title2, display_date, order_num, DAYOFMONTH(modification_date) as modification_date_day, ";
        $feature_list_query .= "MONTH(modification_date) as modification_date_month, YEAR(modification_date) as modification_date_year, ";                          
        $feature_list_query .= "DAYOFMONTH(creation_date) as creation_date_day, MONTH(creation_date) as creation_date_month, ";
        $feature_list_query .= "YEAR(creation_date) as creation_date_year, date_format(modification_date, '%Y-%m-%d %T') as modification_date, language_id ";
        $feature_list_query .= "from feature where category_id=" . $category_id;
 
       $feature_list_query .= " and is_current_version=1 ";

        if (strlen($status) > 0)
        {
            $feature_list_query .= " and status='" . $status . "' ";
        }
        
        
        if ($status=="c")
        {
            $feature_list_query .= "order by order_num desc";
        } else
        {
            $feature_list_query .= "order by creation_date desc";
        }

        $result = $db_obj->query($feature_list_query);
	
	if (strlen($get["regen"])>0){
		while ($fields=array_pop($result)){
			$feature_date= new Date();
$feature_date->set_time(0,0,$fields["creation_date_day"],$fields["creation_date_month"],$fields["creation_date_year"]);
			$category_obj= new Category;
			//echo "regening for day ".$fields["creation_date_day"]." month ". $fields["creation_date_month"]." year ".$fields["creation_date_year"]."<br>";
			$category_obj->cache_archives_for_week($category_id, $feature_date);		
		}
        	$result = $db_obj->query($feature_list_query);	
	}
	

        return $result;
    }

    function get_archives_for_week($category_id, $date)
    {
        //returns a list of all the stories for a given week from a starting date
        //gievn teh feature id

        $db_obj = new DB;

        $date->find_start_of_week();

        $min_date=$date->get_sql_date();
        $date->move_n_weeks_forward(1);
        $max_date=$date->get_sql_date();
        $feature_list_query  = "select ";
	$feature_list_query  .= "YEAR(modification_date) as modification_date_year, ";
        $feature_list_query  .= "MONTH(modification_date) as modification_date_month, ";
        $feature_list_query  .= "DAYOFMONTH(modification_date) as modification_date_day, ";
        $feature_list_query  .= "HOUR(modification_date) as modification_date_hour, ";
        $feature_list_query  .= "MINUTE(modification_date) as modification_date_minute, ";
	$feature_list_query  .= "language_id, ";
	$feature_list_query  .= "feature.* from feature where category_id=" . $category_id;
        $feature_list_query .= " and (status='a' or status='c')";
        $feature_list_query .= " and is_current_version=1";
        $feature_list_query .= " and creation_date>='" . $min_date . "'";
        $feature_list_query .= " and creation_date<'" . $max_date . "'";
        $feature_list_query .= " order by creation_date desc , order_num desc";
        //echo $feature_list_query;
        $result=$db_obj->query($feature_list_query);
        return $result;
    }


    function get_history_list($feature_id)
    {
        //returns a list of info for all the past versions of a story
        //given teh story's id
        $db_obj= new DB;
        global $user_dictionary;

        //$user_dictionary=array();

        $query = "select username, user_id from user";
        $result = $db_obj->query($query);

        while ($next_user=array_pop($result))
        {
            $user_id=$next_user['user_id'];
            $username=$next_user['username'];
            $user_dictionary[$user_id]=$username;
        }
        
        $feature_list_query  = "select version_num, feature_version_id, modifier_id, title1, ";
        $feature_list_query .= "date_format(modification_date, '%W %M %d, %Y at %h:%i %p') as mod_date ";
        $feature_list_query .= "from feature where feature_id=" . $feature_id;
        $feature_list_query .= " order by version_num desc";
        $result = $db_obj->query($feature_list_query);
        return $result;
    }

    function add($featurefields)
    {
        // adds a story to the DB. All info should be handed in in a dictionary
        // object (including the id of the feature to which teh story belongs) 
        $db_obj= new DB;
        global $user_id;

        if (strlen($featurefields['title1'])>0 && is_numeric($featurefields['order_num']))
        {
            $next_key = $db_obj->execute_getnextkey("feature_sequence");

            #in case this is enetered without a logon set user_id to -1
            #this should only happen during debugging

            if (strlen($_SESSION['user_id'])==0)
            {
                $user_id="-1";
            }else{
		$user_id = $_SESSION['user_id'] ;
	    }

            $query  = "INSERT INTO feature ";
            $query .= "(status, creator_id, creation_date, modification_date, modifier_id, feature_id, title1, ";
            $query .= "title2, display_date, tag, image, image_link, summary, order_num, "; 
            $query .= "template_name, category_id, language_id) VALUES ('";
            $query .= $featurefields['status'] . "', ";
            $query .= $user_id . ", ";
            $query .= "'" . $featurefields['creation_date'] . "', now(), " . $user_id . ", ";
            $query .= $next_key . ", '";
            $query .= $featurefields['title1'] . "', '";
            $query .= $featurefields['title2'] . "', '";
            $query .= $featurefields['display_date'] . "', '";
            $query .= $featurefields['tag'] . "', '";
            $query .= $featurefields['image'] . "', '";
            $query .= $featurefields['image_link'] . "', '";
            $query .= $featurefields['summary'] . "', ";
            $query .= $featurefields['order_num'] . ", '";
            $query .= $featurefields['template_name'] . "', ";
            $query .= $featurefields['category_id'] . ", '";
	    $query .= $featurefields['language_id'] . "')";
            $error_num = $db_obj->execute_statement($query);
        } else
        {
            $error_num=1;
        }
        return $error_num;
    }

    function update($featurefields)
    {
        //updates a story given a dictionary object containing all the
        //fields for the story
        //Note that updating is actually setting the old version to be out of date
        //and adding a new row to the db for the new version.

        $db_obj= new DB;
        global $user_id;

        if (strlen($featurefields['feature_version_id']) != 0 && is_numeric($featurefields['order_num']))
        {

            $sql = "select creator_id, creation_date from feature where feature_version_id=" . $featurefields['feature_version_id'];
            $feature_row_rs = $db_obj->query($sql);
            $feature_row = array_pop($feature_row_rs);
            $creator_id = $feature_row['creator_id'];

            if (strlen($creator_id."") == 0)
            {
                $creator_id=-1;
            }

            $creation_date = "'" . $featurefields['creation_date'] . "'";

            if (strlen($creation_date) == 0)
            {
                $creation_date = time();
            }

            $sql = "select max(version_num) as max_version from feature where feature_id=".$featurefields['feature_id'];
            $max_row_rs = $db_obj->query($sql);
            $max_row = array_pop($max_row_rs);
            $old_version_num = $max_row['max_version'];

            $next_version_num=$old_version_num+1;

	    if ($_SESSION['user_id'] < 1)
	    {
	        $user_id = -1;
	    } else
	    {
	        $user_id = $_SESSION['user_id'];
	    }


            $query  = "INSERT INTO feature ";
            $query .= "(status, creator_id, modifier_id, creation_date, modification_date, version_num, feature_id, title1, ";
            $query .= "title2, display_date, tag, image, image_link, summary, order_num, "; 
            $query .= "template_name, category_id, language_id) VALUES ('";
            $query .= $featurefields['status'] . "', ";
            $query .= $creator_id . ", ";
            $query .= $user_id . ", ";
            $query .= $creation_date . ", ";
            $query .= "now() , ";
            $query .= $next_version_num . ", ";
            $query .= $featurefields['feature_id'] . ", '";
            $query .= $featurefields['title1'] . "', '";
            $query .= $featurefields['title2'] . "', '";
            $query .= $featurefields['display_date'] . "', '";
            $query .= $featurefields['tag'] . "', '";
            $query .= $featurefields['image'] . "', '";
            $query .= $featurefields['image_link'] . "', '";
            $query .= $featurefields['summary'] . "', ";
            $query .= $featurefields['order_num'] . ", '";
            $query .= $featurefields['template_name'] . "', ";
            $query .= $featurefields['category_id'] . ", '";
	    $query .= $featurefields['language_id'] . "')";
		
            $error_num = $db_obj->execute_statement($query);
	//	echo $query;
         //	echo $error_num."test";   
	 if ($error_num==0)
            {
                $query = "update feature set is_current_version=0 where ";
                $query .= "feature_id=" . $featurefields['feature_id'];
                $query .= " and version_num !=" . $next_version_num;
	//	echo $query;
                $error_num=$db_obj->execute_statement($query);
            }
        } else
        {
            $error_num=1;
        }
	
	$fields=$this->get_latest_feature($featurefields['feature_id']);
	
	$category_id=$fields["category_id"];
	
	$feature_date=new date();
	$feature_date->set_time(0,0,$fields["creation_date_day"],$fields["creation_date_month"],$fields["creation_date_year"]);
	
	include_once(SF_SHARED_PATH."/classes/category_class.inc");
	
	$category_obj= new Category;
	$category_obj->cache_archives_for_week($category_id, $feature_date);

        return $error_num;
    }

    function delete($feature_id)
    {
        //Deletes a story from the DB (not used as of yet since
        //stories are normally archived for now)
        $db_obj= new DB;

        if (strlen($feature_id)!=0)
        {
            $feature_delete_query = "DELETE FROM feature WHERE feature_id=" . $feature_id;
            $error_num = $db_obj->execute_statement($feature_delete_query);
            $error_num=0;    
        } else
        {
            $error_num=1;
        }

        return $error_num;
    }

    function reorder($posted_fields)
    {    
        //Reorders stories for a gievn feature

        $db_obj= new DB;
        $query1 = "UPDATE feature set creation_date=creation_date, modification_date=modification_date, order_num=";
        $query2 = " WHERE feature_version_id=";

        $querykeys=array_keys($posted_fields);
        $nextkey=array_pop($querykeys);

        while (strlen($nextkey) != 0)
        {
            if (strpos($nextkey, "eature_order"))
            {
                $query  = $query1 . $posted_fields[$nextkey];
                $query .= $query2 . substr($nextkey, 13, strlen($nextkey)-13);

                $db_obj->execute_statement($query);
            }

            $nextkey=array_pop($querykeys);
        }
        return 0;
    }

	/**
	 * Called from within render_feature with an array of fields and values.
	 * Output is a file in site/public/features/cache/json.
	 */
	function render_json( $fields ) 
	{
		$path = SF_ARCHIVE_PATH . '/cache/json/';
		$id = $fields['FEATUREID'];
		$filename = $path.$id.'.json';
		// echo $filename;
		// map fields to something client can use
		$img = strtolower($fields['IMAGE']);
		$ext = substr($img,-4);
		switch($ext) {
			case '.jpg':
			case 'jpeg':
				$mime = 'image/jpeg';
				break;
			case '.png':
				$mime = 'image/png';
				break;
			case '.gif':
				$mime = 'image/gif';
				break;
			default:
				$mime = '';
				break;
		}

		$f = array();
		$f['article'] = array(
			'id'=>$fields['FEATUREID'],
			'heading'=>$fields['TITLE1'],
			'author'=>'',
			'article'=>$fields['SUMMARY'],
			'mime_type'=> $mime,
			'linked_file'=>$fields['IMAGE'],
			'url'=>$fields['IMGLINK']
		);
		$f['comments'] = array();
		$f['attachments'] = array();

		$fh = fopen( $filename, 'w' );
		fwrite( $fh, json_encode($f) );
		fclose( $fh );
	}

    function render_feature($feature_version_id)
    {
        //generates the HTML for a story from its version id
        //the involves running the tempate for the story handing in the 
        //stories fields from the DB
        $db_obj= new DB;
        global $default_feature_template_name;

        $template_obj = new FastTemplate(SF_TEMPLATE_PATH);

        $featurefields = $this->get_feature_fields($feature_version_id);

        $template_name = trim($featurefields['template_name']) . ".tpl";

        if (strlen($template_name) < 5)
        {
            $template_name=$default_feature_template_name . ".tpl";
        }


        if (strlen($template_name) > 0)
        {
            $template_obj->clear_all();
            $template_obj->define(array('feature_row' => $template_name));

            // this is the image tag code, not sure if this is best place for it -gkd
			$imagesize = null;
            if ($featurefields['image'] != "")
            {
                if (file_exists($featurefields['image']))
                {
                    $imagedim = GetImageSize($featurefields['image']);
                    $imagesize = $imagedim[3];
				}
            }

            $defaults = (array(    
                                'TITLE1'      => $featurefields['title1'],
                                'FEATUREID'   => $featurefields['feature_id'],
                                'CRDATE'      => $featurefields['display_date'],
                                'TITLE2'      => $featurefields['title2'],
                                'IMAGE'       => $featurefields['image'],
                                'IMGLINK'     => $featurefields['image_link'],
                                'IMGSIZE'     => $imagesize,
                                'TAG'         => $featurefields['tag'],
                                'SUMMARY'     => $featurefields['summary'],
                                'CATEGORYID'  => $featurefields['category_id']
                                ));

            $template_obj->assign($defaults);
            $template_obj->parse('CONTENT', "feature_row");
            $result_html = $template_obj->fetch("CONTENT");

			$this->render_json( $defaults );
        } else
        {
			$tr = new Translate ;
			$tr->create_translate_table('feature');
            $result_html = $tr->trans('no_template_defined'); 
        }
        return $result_html;
    }

    function updatestatus($feature_version_id, $status){
	include_once(SF_SHARED_PATH."/classes/category_class.inc");
	$db_obj= new DB;
    	
	$query = "update feature set creation_date=creation_date, status='" .$status. "' where feature_version_id=";
        $query .= $feature_version_id;

        $db_obj->execute_statement($query);

	$fields=$this->get_feature_fields($feature_version_id);
	
	$category_id=$fields["category_id"];
	
    	$feature_date=new date();
	$feature_date->set_time(0,0,$fields["creation_date_day"],$fields["creation_date_month"],$fields["creation_date_year"]);
	
	$category_obj= new Category;
	$category_obj->cache_archives_for_week($category_id, $feature_date);
    }

    function get_max_id(){
	$db_obj=new DB;
    	$return_val=0;
	$result=$db_obj->query("Select max(feature_version_id) as id from feature");
	$row=array_pop($result);
	$t_return_val=$row["id"];
	if (strlen($t_return_val)>0){
		$return_val=$t_return_val;
	}	
	return $return_val;
    }

    function get_id_list($min_id, $num_rows, $where){
	$db_obj=new DB;
	$sql="Select feature_version_id as id from feature where feature_version_id>$min_id $where order by feature_version_id limit $num_rows";
	echo $sql;
	$result=$db_obj->query($sql);
	return $result;
    }

}
?>
