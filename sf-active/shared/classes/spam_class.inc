<?php

include_once("shared/global.cfg");
include_once(SF_SHARED_PATH."/classes/spamb_class.inc");


class Spam
{

    function Spam ()
    {
		// Nothing
		error_log('in Spam');
    }
	
	function Detect ($spam_publish)	// Return 1 for a Spam Block
	{
		// return 0; // temporarily disable	

        $post = filter_input_array(INPUT_POST, array(
            'heading' => FILTER_SANITIZE_STRING,
            'summary' => FILTER_SANITIZE_STRING,
            'article' => FILTER_SANITIZE_STRING,
        ));

		$this->Log('Running spam checks.');

		$ret = 0;
		$cspam = 0;
		$ipspam = 0;
		// antispam filter code - st3
		// it'll work only if globals spam_filter_articles and spam_filter_time
		// are set in the local configuration files

		if (isset($GLOBALS['spam_filter_articles']) && isset($GLOBALS['spam_filter_time'])) {

			if ( $GLOBALS['spam_filter_time'] < (time()-filectime(SF_CACHE_PATH.'/hashes_time')) ) {
				unlink(SF_CACHE_PATH.'/hashes.txt');
				touch(SF_CACHE_PATH.'/hashes.txt');
				touch(SF_CACHE_PATH.'/hashes_time');
			}

			if ( 24 * $GLOBALS['spam_filter_time'] < (time()-filectime(SF_CACHE_PATH.'/hashes_content_time')) ) {
				unlink(SF_CACHE_PATH.'/hashes_content.txt');
				touch(SF_CACHE_PATH.'/hashes_content.txt');
				touch(SF_CACHE_PATH.'/hashes_content_time');
			}

			if ($spam_publish) {
				$hashes = fopen(SF_CACHE_PATH.'/hashes.txt','a');
				fputs($hashes,$this->Hash($_SERVER['REMOTE_ADDR'].$_SERVER['HTTP_USER_AGENT'].$_SERVER['HTTP_ACCEPT_LANGUAGE'])."\n");
				fclose ($hashes);
			}
			
			if ($spam_publish) {
				$hashes = file(SF_CACHE_PATH.'/hashes.txt');
				for ( $i=0; $i<count($hashes);$i++)
				{
					if (strcmp($this->Hash($_SERVER['REMOTE_ADDR'].$_SERVER['HTTP_USER_AGENT'].$_SERVER['HTTP_ACCEPT_LANGUAGE']),rtrim($hashes[$i])) == 0) 
					{
					  $matched_hashes++;				  
					}
				}
				if ($matched_hashes > $GLOBALS['spam_filter_articles']) {
				   $ipspam=1;
				   $this->Log("IP Spam Detected");
				}
			}

			// content block by occam
			if ($spam_publish && isset($GLOBALS['spam_filter_content']) ) {

			   $chashes = file(SF_CACHE_PATH.'/hashes_content.txt');
			   for ( $i=0; $i<count($chashes);$i++)
			   {
	        	   if (strcmp( $this->Hash( $post['article'] ) , rtrim($chashes[$i])) == 0) 
				   {
				   	 $matched_chashes++;					 
				   }
			   }
			   if ($matched_chashes > $GLOBALS['spam_filter_content']) { 
			     $cspam=1;
				 $this->Log("Content Spam Detected");
			   }

			   $chashes = fopen(SF_CACHE_PATH.'/hashes_content.txt','a');
			   fputs($chashes, $this->Hash( $post['article'] )."\n");
			   fclose ($chashes);
			}

			
			if ($cspam == 1 || $ipspam == 1 ) $ret=1;			

		}

		// look for specific strings -- johnk
		$sspam = 0;
		if ( $spam_publish && $GLOBALS['spam_filter_strings'] > 0 ) {
			$stringstext = file_get_contents(SF_CACHE_PATH.'/spam_strings.txt');
			$strings = explode("\n",$stringstext);
			foreach ( $strings as $string )
			{
				$string = rtrim( $string );
				if (! $string) 
					continue;
				$lines = explode("\n",$post['heading']."\n".$post['summary']."\n".$post['article']);
				foreach ( $lines as $line )
				{
					if ( preg_match( "/$string/i", $line ) )
					{
						$this->Log("Keyword matches $string");
						$sspam += 1;
						// echo "<font color='white'>matched $string:</font><br />";
					}
				}
			}
		}

		if ( $GLOBALS['spam_filter_strings'] && $sspam >= $GLOBALS['spam_filter_strings'] ) {
			$ret = 1;
			$this->Log("Keyword String Spam Detected");
		}

		
		$lines = file(SF_CACHE_PATH.'/next_ip_to_block.txt'); 	
		$user_ip=trim($_SERVER['REMOTE_ADDR']);
		for ( $i=0 ; $i < count($lines) ; $i++)
		{ 
			$saved_ip = trim($lines[$i]);
		
			//echo strpos( "a".$user_ip,$saved_ip);

			if ( strlen( trim($saved_ip) ) > 0 )
			{
				if 	(strpos("a".$user_ip,$saved_ip)==1) 
				{
					$ret= 1;
					$logline = $_SERVER['REMOTE_ADDR']."|".date("m-d-y g:ia")."|".$_SERVER['HTTP_USER_AGENT']."|".$_SERVER['HTTP_REFERER']."|".$_SERVER['REQUEST_URI'];

					$this->Log($logline);

				} 
			}
		
		}
		
		// look for posts where more than 50% of it is urls or url-like text
		if (false && $spam_publish) {
			$spamb = new SpamB();
			if ($spamb->too_many_urls( $post['article'] ))
				$ret = 1;
		}
				
		return $ret;
	}
	
	
	function Add ($ip) {
	// comming soon for admin interface
	
	}
	
	function Remove ($ip) {
	// comming soon for admin interface	
	
	}
	
	function Log ($text) {
		$log = fopen(SF_CACHE_PATH."/ipblock.log","a");
		fwrite($log, date("m-d-y g:ia").": ".$text."\n");
		fclose($log);	
	
	}

	function log_ip ($article){
		$ip_log_name=SF_CACHE_PATH."/ip_log.txt"; 
		if($GLOBALS['ip_log'] == "ip")
		{
			$user_ip=trim($_SERVER['REMOTE_ADDR']);
		}else{
			$user_ip = "\t";
		}
		$id=$article->article['id'];
		$parent_id=$article->article['parent_id'];
		$title=$article->article['heading'];
		$author=$article->article['author'];
		$time=$article->article['format_created'];
		
		$spamf=fopen($ip_log_name,"a");
		$write="\n$time $user_ip\t$id\t$parent_id\t$author\t$title\n";
		fwrite($spamf, $write);
		fclose($spamf);
	}

	function Hash( $str ){
		$s = strtoupper(preg_replace('/\\s/x','',$str));
		return crc32($s);
	}

}
