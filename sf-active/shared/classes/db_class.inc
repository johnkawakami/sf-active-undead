<?php

if (!isset($db_class_defined))
{
$db_class_defined=true;
$db_debug = false;
$db_debug_save_only_backtraces = true;

class DB
{
    //The DB class is used a a wrapper around all DB access.

    function DB()
    {
        //default constructor for DB class
    }

    function get_connection()
    {
        //Returns a DB connection

        if (strlen(DB_HOSTNAME) > 0)
        {
            $db_host = DB_HOSTNAME;
        } else
        {
            $db_host = "localhost";
        }

        $GLOBALS['db_conn'] = mysql_connect($db_host, DB_USERNAME, DB_PASSWORD);
        mysql_select_db(DB_DATABASE, $GLOBALS['db_conn']);
    }

    function query($sql)
    {
        //runs a SQL statement and returns an list of dictionary objects
        //representing teh resultset. The DB Connection and Resultset are both closed
        //before this function returns
		global $db_debug;
		global $db_debug_save_only_backtraces;
		if ($db_debug) {
				$db_debug_backtrace = false;
				$db_debug_filename = '/tmp/sf-active-db-log-'.getmypid();
				$fh = fopen($db_debug_filename,'a+');
				$time_start = time();
				fwrite($fh,"---start $time_start\n");
				fwrite($fh, $sql);
		}

        if (!isset($GLOBALS['db_conn'])) $this->get_connection();

        $result_prt = mysql_query($sql, $GLOBALS['db_conn']);

		if ($db_debug) {
				$elapsed = time() - $time_start;
				if ($elapsed > 10) {
					$db_debug_backtrace = true;
					fwrite($fh, "\n--------------backtrace-------\n"); 
					fwrite($fh, serialize(debug_backtrace()));
				}	
				fwrite($fh,"\n---end, $elapsed seconds\n");
				fclose($fh);
				//if ($db_debug_save_only_backtraces && !$db_debug_backtrace) 
				//	unlink($db_debug_filename);
		}

        $error_num=$this->error_num($GLOBALS['db_conn']);

        if ($error_num < 0)
        {
			print ("Errror $error_num : ".mysql_error($GLOBALS['db_conn']));
			print ("<p>$sql</p>");
            return $error_num;
        } else
        {
            $result_array = array();
            while ($next_row = mysql_fetch_array($result_prt,MYSQL_ASSOC))
            {
                array_push($result_array, $next_row);
            }
            $result_array = array_reverse($result_array);
            //mysql_close($conn);
            return $result_array;
        }
    }

    function execute_statement($sql)
    {
        //runs a SQL statement that does not have a return value (UPDATE, DELETE, etc..)
        //The DB Connection and Resultset are both closed
        //before this function returns	
		global $db_debug;
		global $db_debug_save_only_backtraces;
		if ($db_debug) {
				$fh = fopen('/tmp/sf-active-db-log-'.getmypid(),'a+');
				$time_start = time();
				fwrite($fh,"---start execute_statement\n");
				fwrite($fh, $sql);
		}


        if (!isset($GLOBALS['db_conn'])) $this->get_connection();
		mysql_query($sql, $GLOBALS['db_conn']);

		if ($db_debug) {
				$elapsed = time() - $time_start;
				if ($elapsed > 10) {
					fwrite($fh, "\n--------------backtrace-------\n"); 
					fwrite($fh, serialize(debug_backtrace()));
				}	
				fwrite($fh,"\n---end, $elapsed seconds\n");
				fclose($fh);
				if ($db_debug_save_only_backtraces && !$db_debug_backtrace) 
					unlink($db_debug_filename);
		}

        $error_num = $this->error_num($GLOBALS['db_conn']);
        //mysql_close($conn);
        return $error_num;
    }

    function execute_statement_return_autokey($sql)
    {
        //runs a SQL statement for an INSERT and returns the ID of the 
        //row added
		global $db_debug;
		global $db_debug_save_only_backtraces;
		if ($db_debug) {
				$fh = fopen('/tmp/sf-active-db-log-'.getmypid(),'a+');
				$time_start = time();
				fwrite($fh,"---start execute_statement_return_autokey \n");
				fwrite($fh, $sql);
		}

        if (!$GLOBALS['db_conn']) $this->get_connection();

        mysql_query($sql, $GLOBALS['db_conn']);

		if ($db_debug) {
				$elapsed = time() - $time_start;
				if ($elapsed > 10) {
					fwrite($fh, "\n--------------backtrace-------\n"); 
					fwrite($fh, serialize(debug_backtrace()));
				}	
				fwrite($fh,"\n---end, $elapsed seconds\n");
				fclose($fh);
				if ($db_debug_save_only_backtraces && !$db_debug_backtrace) 
					unlink($db_debug_filename);
		}

        $error_num=$this->error_num($GLOBALS['db_conn']);
        if ($error_num==0)
        {
            $result_num=mysql_insert_id($GLOBALS['db_conn']);
        } else
        {
			print ("Errror $error_num : ".mysql_error($GLOBALS['db_conn']));
			print ("<p>$sql</p>");
            $result_num=$error_num;
        }
        //mysql_close($conn);
        return $result_num;
    }


    function execute_getnextkey($sequence_table)
    {
        //treats a MySQL tabel as a sequence by inserting
        //and empty row and getting thr row id

        if (!$GLOBALS['db_conn']) $this->get_connection();

        $sql = "insert into ".$sequence_table." values()";
        mysql_query($sql, $GLOBALS['db_conn']);

        $next_id_row = mysql_insert_id();
        //mysql_close($conn);

        return $next_id_row;
    }

    function error_num()
    {
        //returns an error num as a negative number to distinguish it
        //from an id (in the case of inserts)
        return -1*mysql_errno($GLOBALS['db_conn']);
    }

    function close()
    {
        mysql_close($GLOBALS['db_conn']);
    }
	function escape($s) 
	{
        if (!$GLOBALS['db_conn']) $this->get_connection();
		return mysql_real_escape_string($s,$GLOBALS['db_conn']);
	}
	function quote($s)
	{
		return "'".$this->escape($s)."'";
	}
}
}
?>
