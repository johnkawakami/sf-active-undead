<?php
// Refreshes the local cities list based on global XML master.
// Based on some dadaIMC code
//     license: GNU LGPL
//     copyright 2001-2002: dada typo and contributors
class admin_cities extends Page
{
    function admin_cities ()
    {
        return 1;
    }

	function execute()
	{
	$GLOBALS['cities_lang'] = '';
		foreach($GLOBALS['i18n_cities'] as $key)
		{
			if($key == "en")
			{
				$GLOBALS['cities_lang'] = '';
			}
			else
			{
				$GLOBALS['cities_lang'] = $key.'.';
			}
			$xml = new XML;
		}
	}
}
class XML
{
	function XML()
	{
		$pathtofile = SF_SHARED_PATH.'/cities.'.$GLOBALS['cities_lang'].'inc';
        $file = 'http://contact.indymedia.org/cities.'.$GLOBALS['cities_lang'].'xml';
        $GLOBALS['depth'] = 0;
        $GLOBALS['currenttag'] = '';
        $GLOBALS['items'] = array();
        $GLOBALS['listing'] = array();


// use case-folding so we are sure to find the tag in $map_array
        //xml_parser_set_option($this->xml_parser, XML_OPTION_CASE_FOLDING, true);
		$this->parser = xml_parser_create();
		xml_set_object($this->parser, &$this);
        xml_set_element_handler($this->parser,"startElement", "endElement");
        xml_set_character_data_handler($this->parser, "characterData");
		$this->parse($data, $file, $pathtofile);


		}



	function startElement($parser, $name, $attrs)
    {
	$GLOBALS['currenttag'] = $name;
	if ($GLOBALS['currenttag'] == 'DICT')
        {
            $GLOBALS['depth']++;
        }
    }
    function endElement($parser, $name)
    {
        if ($GLOBALS['depth'] == 3 && count($GLOBALS['items']) >= 2)
        {
$GLOBALS['listing'][$GLOBALS['currentcat']][$GLOBALS['currentregion']][]=$GLOBALS['items'];
	    $GLOBALS['items'] = array();
	}
	if ($name == 'DICT')
        {
            $GLOBALS['depth']--;
        }
	$GLOBALS['currenttag'] = '';
    }
    function characterData($parser, $data)
    {
    	if ($GLOBALS['depth'] == 1 && $GLOBALS['currenttag'] == 'KEY')
        {
     	    $GLOBALS['currentcat'] = $data;
            $GLOBALS['listing'][$GLOBALS['currentcat']] = array();
    	}
        elseif ($GLOBALS['depth']==2 && $GLOBALS['currenttag']=='KEY')
        {
     	    $GLOBALS['currentregion'] = $data;
$GLOBALS['listing'][$GLOBALS['currentcat']][$GLOBALS['currentregion']]=array();
    	}
        elseif ($GLOBALS['depth'] == 3)
        {
     	    if ($GLOBALS['currenttag']=='KEY' || $GLOBALS['currenttag']=='STRING')
            {
      	        if (isset($GLOBALS['items'][$GLOBALS['currenttag']]))
                {
       	            $GLOBALS['items'][$GLOBALS['currenttag']]=$GLOBALS['items'][$GLOBALS['currenttag']].$data;
        	}
                else
                {
     		    $GLOBALS['items'][$GLOBALS['currenttag']] = $data;
        	}
            }
    	}
    }

	function parse($data, $file, $pathtofile)
	{
		if (!($fp = fopen($file, "r")))
        {
	    $tr = new Translate ;
	    $trans_die = $tr->trans('die');
            die($trans_die);
        }
        while ($data = fread($fp, 2048))
        {
            if (!xml_parse($this->parser, $data, feof($fp)))
            {
                die(sprintf("XML error: %s at line %d. with lang".$GLOBALS['cities_lang'],
                            xml_error_string(xml_get_error_code($this->parser)),
                            xml_get_current_line_number($this->parser)));
            }
        }
        xml_parser_free($this->parser);
        if (!empty($GLOBALS['listing']))
        {
            $started = false;
	    $str = '';
	    foreach($GLOBALS['listing'] as $cat=>$arr)
            {
		if ($started) $str .= "\n";
	       	if ($cat != 'NULL')
                {
		    $str .= "\n<b>".$cat.":</b>\n";
        	}
	        $started2 = false;
	        foreach($arr as $reg=>$imcarr)
                {
	            if ($started2) $str .= "\n";
		    if ($cat == 'NULL' && $reg != 'NULL')
                    {
	                $str .= "\n<b>".$reg.":</b>\n";
        	    }
	            foreach($imcarr as $i=>$data)
                    {
		        $str .= '<a href="'.$data['STRING'].'">'.strtolower($data['KEY'])."</a> ";
		    }
        	    $started2 = true;
	        }
                //$str = str_replace('&', '&amp;', $str);
                $str = str_replace('&amp;amp;', '&amp;', $str);
	    $started = true;
            }
	    $str .= "\n";
        }
        else
        {
	$tr = new Translate ;
        $str = $tr->trans('cities_unavailable') ;
        }
        $fp = fopen($pathtofile,'w');
        fwrite($fp,$str);
        fclose($fp);
        $this->tkeys['local_cities']=implode('',file(SF_SHARED_PATH.'/cities.'.$GLOBALS['cities_lang'].'inc'));



	}
}



?>
