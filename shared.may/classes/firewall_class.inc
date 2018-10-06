<?php // vim:ai:ts=4:sw=4:st=4:et

/**
 * Static class to add IP addresses to the firewall table,
 * and see if the IP address is in the table.
 *
 * To create the table for this:

   CREATE TABLE `firewall` (
   `ip` int(11) NOT NULL default '0',
     PRIMARY KEY  (`ip`)
     ) ENGINE=MyISAM DEFAULT CHARSET=binary COMMENT='ip addresses to block';
 */
class Firewall {

    /**
     * Adds an IP address to the table.
     */
	public static function add($ip) {
		$db_obj = new \SFACTIVE\DB;
		$ipint = ip2long($ip);
		if ($ipint) {
			$db_obj->execute_statement("INSERT INTO firewall VALUES(".$ipint.")");	
			Firewall::exit500();
		}
	}

    /**
     * Checks if an address is blocked.
     */
	public static function isBlocked($ip) {
		$db_obj = new \SFACTIVE\DB;
		$ipint = ip2long($ip);
		if ($ipint) {
			$result = $db_obj->query("SELECT COUNT(*) AS ct FROM firewall WHERE ip=".$ipint);	
			return (intval($result[0]['ct'])==1);
		}
		return false;
	}

    /**
     * Checks if the client's IP address is blocked,
     * and exits the script if it is, sending a 500 status code.
     * 
     * This is the most common use case for this class.
     */
	public static function protect() {
		if (Firewall::isBlocked($_SERVER['REMOTE_ADDR'])) { 
			http_response_code(500); 
			exit(); 
		}
	}

	public static function exit500() {
		http_response_code(500);
		exit();
	}
}
