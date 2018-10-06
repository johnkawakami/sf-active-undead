<?php
namespace SFACTIVE;

class Language extends \Page 
{

  function __construct()
  {
    return 1 ;
  }

  function language_list()
  {
    $db = new \SFACTIVE\DB();
    $sql = "select id, name, language_code, order_num, display, build from language order by order_num asc" ;
    $result = $db->query($sql);
    return $result;
  }


  function language_edit($id)
  {
    $db = new \SFACTIVE\DB;
    $sql = "select name, language_code, order_num, display, build from language where id = '".$id."'";
    $result_num = $db->query($sql);
    return $result_num ;
  }

  function update($languagefields)
  {
	$db = new \SFACTIVE\DB;
	$sql = "update language set name = '".$languagefields['name']."', language_code = '".$languagefields['language_code']."', order_num = '".$languagefields['order_num']."', display = '".$languagefields['display']."', build = '".$languagefields['build']."' where id = '".$languagefields['id']."'";
	$result = $db->execute($sql);
	return $result;
  }

  function add($languagefields)
  {
    $db = new \SFACTIVE\DB;
	$sql = "insert into language values ('', '".$languagefields['name']."', '".$languagefields['language_code']."', '".$languagefields['order_num']."', '".$languagefields['display']."', '".$languagefields['build']."')";
	$result = $db->execute($sql);
	return $result;
  }

  function hide($languagefields)
  {
    $db = new \SFACTIVE\DB;
	$sql = "update language set display = 'f' where id = '".$languagefields['id']."'";
	$result = $db->execute($sql);
	return $result;
  }

  function show($languagefields)
  {
    $db = new \SFACTIVE\DB;
	$sql = "update language set display = 't' where id = '".$languagefields['id']."'";
	$result = $db->execute($sql);
	return $result;
  }

    function cache_language_select()
    {
    $db = new \SFACTIVE\DB;
	$sql = "select name, id from language where display = 't' order by order_num asc";
	$result = $db->query($sql);
	$file = "<?php\n $"."language_options = array(\n";
	while($row = array_pop($result))
	{
	  $file .= "  ".$row['id']." => \"".$row["name"]."\",\n";
	}
	$file .= ");\n?>";
	$this->cache_file($file, SF_CACHE_PATH."/language_options.inc");
    }

    function reorder($posted_fields)
    {
	$db = new \SFACTIVE\DB();
	$query1="update language set order_num=";
	$query2=" where id=";
	$querykeys=array_keys($posted_fields);
	$nextkey=array_pop($querykeys);
	while (strlen($nextkey)!=0)
	{
		if (preg_match("/order_num/", $nextkey))
		{
			$query=$query1.$posted_fields["$nextkey"];
			$query=$query.$query2.substr($nextkey,9,strlen($nextkey)-9);
			$db->execute($query);
		}
		$nextkey=array_pop($querykeys);
	}
	return 0;
    }

    function cache_builds()
    {
	$db = new \SFACTIVE\DB ;
	$query = "select id, name from language where display = 't' and build = 'y' order by order_num asc";
        $result = $db->query($query);
        $file = "<?php\n";
        while($row = array_pop($result))
        {
            $language_id = $row['id'];
	    $language = $row['name'];
            $file .= "\t$"."GLOBALS['build_sites'][$language_id] = \"".$language."\";\n";
        }
        $file .= "\n?>\n";
        $this->cache_file($file, SF_CACHE_PATH."/build_sites.inc");
        return 1 ;
    }

    function cache_language_codes()
    {
        $db = new \SFACTIVE\DB ;
        $query = "select id, language_code from language where display = 't' order by order_num asc";
        $result = $db->query($query);
        $file = "<?php\n";
        while($row = array_pop($result))
        {
            $file .= "\t$".'language_codes[$row[\'id\']]'." = \"".$row['language_code']."\";\n";
        }
        $file .= "\n?>\n";
        $this->cache_file($file, SF_CACHE_PATH."/language_codes.inc");
        return 1 ;
    }

    function get_language_code($language_id)
    {
	global $language_codes ;
	return $language_codes[$language_id];
    }
	
}
