<?php
include_once("shared/global.cfg");

class Backup 
{

// Get the content of $table as a series of INSERT statements.
// After every row, a custom callback function $handler gets called.
// $handler must accept one parameter ($sql_insert);
function get_table_content($table, $sql)
{
    $db_obj= new DB;
    global $db_setup;
    $conn=$db_obj->get_connection(); 
    $result=mysql_db_query($db_setup["database"], $sql, $conn);
    $i = 0;
    $myres="";
    while($row = mysql_fetch_row($result))
    {
        set_time_limit(60); // HaRa
        $table_list = "(";

        for($j=0; $j<mysql_num_fields($result);$j++)
            $table_list .= mysql_field_name($result,$j).", ";

        $table_list = substr($table_list,0,-2);
        $table_list .= ")";

        if(isset($GLOBALS["showcolumns"]))
            $schema_insert = "INSERT INTO $table $table_list VALUES (";
        else
            $schema_insert = "INSERT INTO $table VALUES (";

        for($j=0; $j<mysql_num_fields($result);$j++)
        {
            if(!isset($row[$j]))
                $schema_insert .= " NULL,";
            elseif($row[$j] != "")
                $schema_insert .= " '".addslashes($row[$j])."',";
            else
                $schema_insert .= " '',";
        }
        $schema_insert = ereg_replace(",$", "", $schema_insert);
	$schema_insert=str_replace("\n","\\n",$schema_insert);
        $schema_insert=str_replace("\r","\\r",$schema_insert);
	$schema_insert .= ");\n";
        $myres.=$schema_insert;
	$i++;
    }
    mysql_close($conn);
    return $myres;
}
}
?>
