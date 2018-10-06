<?php

include_once("shared/global.cfg");

/* archive.org upload class - use it in a script or your fav CMS - this one works in sf-active 

Example:
define('IA_FTP_SERVER', 'audio-uploads.archive.org');
define('IA_FTP_USER', 'user');
define('IA_FTP_PASS', 'pass');

include_once (SF_CLASS_PATH.'/push2ia.inc');

$ia = new push2ia();
$ia->pushFile("ia_id","/tmp/test.file");

*/

class push2ia {

	 var $ftp_server;
	 var $ftp_user_name;
	 var $ftp_user_pass;

	function push2ia () {
		/* set connectio data from the config, maybe you have to change this for your script */
		$this->ftp_server = IA_FTP_SERVER;
	 	$this->ftp_user_name = IA_FTP_USER;
	 	$this->ftp_user_pass = IA_FTP_PASS;
	}

	function pushFile ($ia_id,$file) {

		$dirname = $ia_id;
		$localfile = $file;
		$remotefile = substr( $file, strrpos($file , "/") + 1);

		echo "Connect...\n";
		$conn_id = ftp_connect($this->ftp_server);

		echo "Login...\n";
		$login_result = ftp_login($conn_id, $this->ftp_user_name, $this->ftp_user_pass);
		
		echo "Pasive...\n";
		ftp_pasv($conn_id, TRUE);
		
		echo "Mkdir $dirname \n";
		ftp_mkdir($conn_id, $dirname);
		
		echo "ChDir $dirname \n";
		ftp_chdir($conn_id, $dirname);
		
		echo "Put $localfile -> $remotefile \n";
		ftp_put($conn_id, $remotefile, $localfile, FTP_BINARY);
		
		echo "Closing..\n\n";
		ftp_quit($conn_id); 
	}

	function updateMetadata ($ia_id) {

	}
	
	function getNotMirroredFiles () {
		$db = new DB;
		$query = 'select * from webcast where (mirrored != "t") and (mime_type like "video/%")';
		$resultset = $db->query($query);
		
		return $resultset;
	}
	
	function updateMirrorStatus ($id) {
	
	}

}

?>
