<?php

/**
* KnowMore Class - a class to screen scrape knowmore.org, and link
* company names to the knowmore.org database.
*
* Copyright 2006 John Kawakami
*
* This class is licensed under the terms of the GNU General Public License.
* For details, see http://gnu.org/copyleft/gpl.html
*
* Version 1 - 2-Aug-2006
*/
class KnowMore
{
    var $text;
    var $data;
    function KnowMore()
    {
        if (file_exists('/tmp/knowmore'))
        {
            $this->text = file_get_contents( '/tmp/knowmore' );
            $this->getAllTitles();
            return;
        }
        $this->text = file_get_contents( 'http://www.knowmore.org/index.php?title=Special:Browse&filter=all&id=1' );
        $fh = fopen( '/tmp/knowmore', 'w' );
        fwrite( $fh, $this->text );
        fclose($fh);
        $this->getAllTitles();
    }
    /**
     * Scrapes the data into an array.
     */
    function &getAllTitles()
    {
        if (file_exists('/tmp/knowmore.db'))
        {
            $this->data = unserialize( file_get_contents('/tmp/knowmore.db') );
            return $this->data;
        }
        // else parse the data and save it
        $this->text = file_get_contents( '/tmp/knowmore' );
        $this->data = array();
        $count = preg_match_all( '/a href="([^>]+?)" title ="(.+?)"/im', $this->text, $matches, PREG_SET_ORDER );
        // print_r($matches);
        reset($matches);
        foreach($matches as $match)
        {
            $this->data[] = array( $match[1], $match[2] );
        }
        $fh = fopen( '/tmp/knowmore.db', 'w' );
        fwrite( $fh, serialize($this->data) );
        fclose($fh);
        return $this->data;
    }
    function asJsArray()
    {
        reset($this->data);
        foreach( $this->data as $link )
        {
            if ($result) $result .= ', ';
            $result .= '[ \''. addslashes( $link[1] ) .'\', ';
            $result .= '\''. 'http://knowmore.org' . $link[0] .'\' ]';
        }
        return '['.$result . '];';
    }
    /**
     * Returns the URL to the first company that matches $string.
     * If it's not an exact match, it'll try to find an inexact match.
     */
    function select( $string )
    {
        if ($this->data[$string]) return $this->data[$string];
        
        // normalize search string
        $string = preg_replace( "/('|\\.|\\/|,|Inc.| )/", '', $string );

        foreach( $this->data as $value )
        {
            list( $url, $name ) = $value;
            if (preg_match( "/$string/i", $name))
                return $value;
            // normalize company name
            $modified = preg_replace( "/('|\\.|\\/|,| )/", '', $name );
            if (preg_match( "/$string/i", $modified ))
                return $value;
        }
    }

    /**
     * Scans text for strings bewteen [braces] and converts them
     * into knowmore.org links if the company exists on knowmore.org.
     */
    function knoWiki( $string )
    {
        $delimRE = '\\[.+?\\]';

           preg_match_all( "/$delimRE/i", $string, $lnk );
           $size = sizeof($lnk[0]);
           $i = 0;
           while ($i < $size) {
               $name = ltrim( rtrim( $lnk[0][$i], ']'), '[' );
            if ( $value = $this->select($name) )
            {
                list( $url, $fullname ) = $value;
                $string = str_replace( $lnk[0][$i], "<a href='http://knowmore.org$url'>$name</a>", $string );
            }
               $i++;
           }
        return $string;
    }
}
?>
