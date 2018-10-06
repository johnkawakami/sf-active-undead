<?php // vim:et:ai:ts=4:sw=4

namespace SFACTIVE;

use Monolog\Logger;
use Monolog\Handler\StreamHandler;
use PDO;
use Exception;

function warning_handler($errno, $errstr) { 
    throw new \Exception( $errstr, $errno ); 
}

class DB
{
    function __construct($debug = false)
    {
        $this->debug = $debug;
        $this->db = false;
        $this->log = new Logger('db');
        $this->log->pushHandler(new StreamHandler('/tmp/sf-active-db-log'));
    }

    function getConnection()
    {
        $db_host = !defined('DB_HOSTNAME') ? 'localhost' : DB_HOSTNAME;

        if (!$this->db) { 
            $dbpass = DB_PASSWORD;
            $this->db = new PDO('mysql:host='.$db_host.';dbname='.DB_DATABASE.';charset=utf8', DB_USERNAME, $dbpass);
        }
        return $this->db;
    }

    function queryFetchOne($sql, $context = [])
    {
        $db = $this->getConnection();
        $stmt = $db->prepare($sql);
        if ($stmt->execute($context)) { 
            $value = $stmt->fetchColumn();
            $stmt->closeCursor();
            return $value;
        } else {
            return false;
        }
    }

    function query($sql, $context = [])
    {
        //runs a SQL statement and returns an list of dictionary objects
        //representing teh resultset. The DB Connection and Resultset are both closed
        //before this function returns
		if ($this->debug) {
            $this->log->debug($sql);
            $time_start = time();
		}

        $db = $this->getConnection();
        $stmt = $db->prepare($sql);
        if ($stmt->execute($context)) {
            $result_array = array_reverse($stmt->fetchAll());
            $stmt->closeCursor();
        } else {
            if ($this->debug) { $this->log->debug('SFACTIVE\\DB->query() failed'); }

            throw new Exception('SFACTIVE\\DB->query() failed '. $sql);
        }

		if ($this->debug) {
            if ((time() - $time_start) > 10) {
                $this->log->debug("\n--------------backtrace-------\n"); 
                $this->log->debug(serialize(debug_backtrace()));
            }	
		}
        
        return $result_array;
    }

    function execute($sql, $context = [] )
    {

        //runs a SQL statement that does not have a return value (UPDATE, DELETE, etc..)
        //The DB Connection and Resultset are both closed
        //before this function returns	
		if ($this->debug) {
            $time_start = time();
            $this->log->debug($sql);
		}

        // set_error_handler('SFACTIVE\warning_handler', E_WARNING);

        $db = $this->getConnection();
        $stmt = $db->prepare($sql);
        if ( $context ) {
            $result = $stmt->execute($context);
        } else {
            $result = $stmt->execute();
        }

        // restore_error_handler();

        if ( $result ) {
            $stmt->closeCursor();
            return null;
        } else {
            return $stmt->errorCode();
        }
    }

    /**
     * Runs an update query and returns the number of rows affected.
     */
    function update($sql, $context = []) 
    {
        $db = $this->getConnection();
        $stmt = $db->prepare($sql);
        if ($stmt->execute($context)) {
            $count = $stmt->rowCount();
            $stmt->closeCursor();
            return $count;
        } else {
            return false;
        }
    }

    function insert($sql, $context = [])
    {
        //runs a SQL statement for an INSERT and returns the ID of the 
        //row added
		if ($this->debug) {
            $time_start = time();
            $this->log->debug($sql);
		}

        $db = $this->getConnection();
        $stmt = $db->prepare($sql);
        if ($stmt->execute($context)) {
            return $db->lastInsertId();
        } else {
            //  log $stmt->errorInfo()[2];
            return false;
        }
    }


    /**
     * Semantic change for 7.  Finds the next highest sequence for a given field.
     */
    function execute_getnextkey($sequence_table, $field='id')
    {
        $db = new \SFACTIVE\DB();
        return $db->queryFetchOne('SELECT MAX(`order_num`)+1 FROM `webcast`');
    }

    function close()
    {
        $this->db = null;
    }

	function escape($s) 
	{
        throw new Exception("Don't use escape. Use PDO-style binding. $s");
	}
	function quote($s)
	{
		return "'".$this->escape($s)."'";
	}
}

