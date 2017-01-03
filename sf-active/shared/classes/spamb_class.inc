<?php

include_once("shared/global.cfg");

/**
 * This is a lot like Spam, except it'll operate on arbitrary strings, not just articles.
 * The different spam detection methods are in separate functions.
 */
class SpamB
{

    function SpamB ()
    {
		// Nothing
    }
	
	function detect_strings( &$data )
	{
		$matches = 0;
		$stringstext = file_get_contents(SF_CACHE_PATH.'/spam_strings.txt');
		$strings = explode("\n",$stringstext);
		foreach ( $strings as $string )
		{
			$string = rtrim( $string );
			if (! $string) 
				continue;
			$lines = explode( "\n", $data );
			foreach ( $lines as $line )
			{
				if ( preg_match( "/$string/i", $line ) )
				{
					$matches = $matches + 1;
				}
			}
		}
		return $matches;
	}
	function detect_ip( $ip )
	{
		$lines = file(SF_CACHE_PATH.'/next_ip_to_block.txt'); 	
		$user_ip=trim($_SERVER['REMOTE_ADDR']);
		for ( $i=0 ; $i < count($lines) ; $i++)
		{ 
			$saved_ip = rtrim(trim($lines[$i]));
		
			if ( preg_match( "/^$saved_ip\$/", $user_ip ) )
			{
				$this->Log( $_SERVER['REMOTE_ADDR']."|".
					date("m-d-y g:ia")."|".
					$_SERVER['HTTP_USER_AGENT']."|".
					$_SERVER['HTTP_REFERER']."|".
					$_SERVER['REQUEST_URI'] );
				return 1;
			}
		}
		return 0;
	}
	/** not tested */
	function remember_content( &$data )
	{
		if ( $GLOBALS['spam_filter_time'] < (time()-filectime(SF_CACHE_PATH.'/hashes_time')) ) {
			unlink(SF_CACHE_PATH.'/hashes.txt');
			touch(SF_CACHE_PATH.'/hashes.txt');
			touch(SF_CACHE_PATH.'/hashes_time');
			unlink(SF_CACHE_PATH.'/hashes_content.txt');
			touch(SF_CACHE_PATH.'/hashes_content.txt');
		}
		$hashes = fopen(SF_CACHE_PATH.'/hashes.txt','a');
		fputs( $hashes, md5($data)."\n" );
		fclose ($hashes);
	}
	/**
	 * @return the number of matches
	 * not tested
	 */
	function detect_repeated_content( &$data )
	{
		$hashes = file(SF_CACHE_PATH.'/hashes_content.txt','r');
		$dataHash = md5($data);
		$matched_hashes = 0;
		foreach( $hashes as $hash )
		{
	    	if ( $hash == $dataHash )
				$matched_hashes++;
		}
		return $matched_hashes;
	}
	
	function Log ($text) {

		$log = fopen(SF_CACHE_PATH."/ipblock.log","a");
		fwrite($log, date("m-d-y g:ia").": ".$text."\n");
		fclose($log);	
	}
	
	function too_many_urls( $str ) {
		return 0 ; // disable this feature for now
		$sansUrls = preg_replace( '/http:[a-zA-Z0-9.\\/?=]+/', '', $str );
		$sansHref = preg_replace( '/href/', '', $sansUrls );
		$sansUrl = preg_replace( '/url/', '', $sansHref );
		$sansWww = preg_replace( '/www[a-zA-Z0-9.\\/]+/', '', $sansUrl );
		if (strlen($str)==0)
			return 0;
		if ((strlen($sansWww)/strlen($str)) < 0.5)
			return 1;
		else
			return 0;
	}
}
