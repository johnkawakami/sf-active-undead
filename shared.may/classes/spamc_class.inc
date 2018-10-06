<?php

/* This departs from the Spam and SpamB classes, with a totally different 
interface, and different methodology.  It basically replaces the SpamB::detect_strings() 
functionality with something based on textual analysis.
*/

// ugh- ugly class. needs proper design.
class SpamC {
  var $text;
  var $word_count;
  var $keyword_counts; // all the keywords
  var $keywords; // the top 10 keywords
  var $keyword_density;
  var $keyword_shares;
  var $spam;
  var $matches;
  var $names;
  function SpamC($text) 
  {
    $this->spam = null;
    $this->text = $text;
    $ta = $this->ta = new TextAnalyzer();
    $wc = $this->word_count = $ta->word_count($text);
    $kw = $this->keyword_counts = $ta->keyword_counts($text);
    $keywords = $ta->filter_out_singletons($kw);
    $this->keywords = array_slice($keywords, 0, 20);
    $ratios = array();
    foreach($this->keywords as $word=>$count) 
    {
      $ratios[$word] = $count / $wc;
    }
    $this->keyword_density = $ratios;

    $this->keyword_shares = $ta->histogram_percentages($this->keywords);

	$this->names = $ta->find_names($text);
  }
  function spam_score() 
  {
    if ($this->spam != null) return $this->spam;

    $spam = 0;
    $r = array_values($this->keyword_density);
    if ($r[0]>0.1) { $spam++; }
    if ($r[1]>0.1) { $spam++; }

    $s = array_values($this->keyword_shares);
    if ($s[0]>0.2) { $spam++; }

    // keyword detection part
    $stringstext = file_get_contents(SF_CACHE_PATH.'/spam_strings.txt');
    $strings = explode("\n",$stringstext);
    $strings = array_map('trim',$strings);
    $this->matches = $this->ta->keyword_matches(array_flip($strings), array_keys($this->keywords));
    $spam += count($this->matches);
    $this->spam = $spam;
    return $this->spam;
  }
  function is_spam()
  {
    if ($this->spam_score() > 2) return true;
  }
}

//this class needs to be made OO
class TextAnalyzer {

  var $common;
  var $topic_words;
  var $connector_words;

  function TextAnalyzer()
  {
    $common_words = array('the','be','to','of','and','a','in','that','have','I','it','for','not','on','with', 
    'he','as','you','do','at','this','but','his','by','from','they','we','say','her','she','or','an','will',
    'my','one','all','would','there','their','what','so','up','out','if','about','who','get','which','go',
    'me','when','make','can','like','time','no','just','him','know','take','people','into','year','your',
    'good','some','could','them','see','other','than','then','now','look','only','come','its','over','think',
    'also','back','after','use','two','how','our','work','first','well','way','even','new','want','because',
    'any','these','give','day','most','us','is','are',"don't",'has','was', 'had', 'has', 'there are', 'made',
    'by the','of a','to the','of the','on the','and the','in the','has also','for this', 'will be', 'am', 'pm',
    'which is','in a','not to','but it','is that','that is', 'with no', 'without','took', 'were', 'i',  'we were', 
	'angeles', 'at', 'been', 'by wgk', 'by', 'city', 'event', 'join', 'la', 'states', 'more', 'th', 'at',
	'near', 'above', 'before', 'community', 'living', 'never', 'hate', 'los', 'wasn\'t', 'many',
	'asked', 'being', 'group', 'main', 'began', 'rally at', 'at 6th',
    'said',
	'talk on', 'a second time', 'advisory', 'main room', 'action', 'be able to',
	'her family', 'will be featuring', 'be featuring', 'sold', 'plaza',
	'several', 'on june', 'supervisors', 'still', 'important', 'civic', 'as well',
	'along', 'action alert', 'on march', 'same', 'recent', 'serial', 'while',
	'board', 'he then', 'let go', 'he was', 'proceeded', 'permission', 'down',
	'he could', 'told him', 'camera', 'man', 'we', 'san', 'press', 'wells', 'executives',
	'meeting', 'wells', 'area', 'activist', 'stop', 'person', 'under', 'those',
	'we are', 'activists', '1', 'west', 'percent', 'told', 'organizers', 'father',
	'house', 'customers', 'sort', 'home', 'microphone', 'young', 'crises', 'learn more',
	'while ms', 'activists announce', 'de los', 'de las', 'del', 'para', 'que',
	'las', 'as a', 'are a', 'there', 'local', 'going', 'great', '0', 'they were'
	);
    $this->common = array();
    foreach($common_words as $word)
      $this->common[$word] = 1;
    $this->topic_words = array('immigration'=>1,'immigrant'=>1,'action'=>1,'police'=>1,'environment'=>1,'liberation'=>1,
    'undocumented'=>1,'election'=>1,'gay'=>1,'environment'=>1,'pesticides'=>1,'galvus'=>1);
    $this->connector_words = array('of'=>1,'in'=>1,'is'=>1,'which'=>1,'that'=>1,'for'=>1,'and'=>1,'but'=>1,'to'=>1,'the'=>1,'from'=>1, 'by'=>1 );
  }

  function histogram_percentages( $hash )
  {
    $total = array_sum(array_values($hash));
    $o = array();
    foreach($hash as $k=>$v)
      $o[$k] = $v/$total;
    return $o;
  }

  function keyword_counts( $text ) 
  {
    $connector_words = $this->connector_words;
    $common = $this->common;
    $o = array();
    $text = strtolower($text);
    $text = preg_replace("/['’]s/",'',$text); // no possessives
    $text = preg_replace("/[\x80-\x9f%\$&;:\"“”\(\),.~\/-]/",'',$text);
    $text = preg_replace("/[\r\n]+/",' ',$text);
		// weird bug
    $text = preg_replace("/\\n/",' ',$text);
    $words = explode(' ',$text);
	$words = array_map( create_function('$a','return rtrim(ltrim($a));'), $words );
    
    // now make an array of pairs
    $pairs = array();
    $last_word = '';
    foreach($words as $word)
    {
      if ($word  and !$connector_words[$word] and !$connector_words[$last_word]) array_push($pairs, $last_word . ' ' . $word);
      $last_word = $word;
    }
    foreach($pairs as $pair)
    {
      if (! $common[$pair])
      {
        if (!$o[$pair]) $o[$pair]=0;
        $o[$pair]++;
      }
    }

    // now make an array of triplets
    $triplets = array();
    $word2 = $word3 = '';
    foreach($words as $word3)
    {
      if ($word1 and $word2 and $word3 and !$connector_words[$word1] and !$connector_words[$word2] and !$connector_word[$word3]) array_push($triplets, $word1 . ' ' . $word2 . ' ' . $word3);
      $word1 = $word2;
      $word2 = $word3;
    }
    foreach($triplets as $word)
    {
      if (! $common[$word])
      {
        if (!$o[$word]) $o[$word]=0;
        $o[$word]++;
      }
    }

    foreach($words as $word)
    {
      if (! $common[$word] and (strlen($word)>2) ) 
      {
        if ($o[$word])
          $o[$word]++;
        else 
          $o[$word] = 1;
      }
    }
    unset($o['']);
    arsort($o);
    return $o;
  }

  function filter_out_singletons( $o )
  {
    foreach($o as $key=>$value)
      if ($value == 1)
        unset($o[$key]);
    return $o;
  }

  function keyword_matches( $topics, $keywords )
  {
    $t = array();
    foreach($keywords as $word)
    {
      $word = strtolower($word);
      if (isset($topics[$word]))
      {
        if ($t[$word])
          $t[$word]++;
        else
          $t[$word] = 1;
      }
    }
    return $t;
  }

  function word_count($text)
  {
    $text = strtolower($text);
    $text = preg_replace("/['’]s/",'',$text); // no possessives
    $text = preg_replace("/[&;:\"“”\(\),.~]/",'',$text);
    $text = preg_replace("/[\n\r]+/",' ',$text);
    $words = explode(' ',$text);
    return sizeof($words);
  }

  function find_names($text) 
  {
	$text = $this->replace_accents($text);
	// replace initial caps on sentences with lowercase
	$text = preg_replace( '/\\.\\s\\s([A-Z])/', '.  '.strtolower("$1"), $text );
	$text = preg_replace( '/^([A-Z])/m', strtolower("$1"), $text );
  	preg_match_all('/[A-Z]([a-z]+|\\.)(?:\\s+[A-Z]([a-z]+|\\.))*(?:\\s+(de|de la|del|of)){0,1}\\s+[A-Z]([a-z]+|\\.)/', $text, $matches);
	$names = $matches[0];
	$o = array();
	$words = $this->connector_words;
	$words['The'] = 1;
	$words['and'] = 1;
	//print_r(array_keys($words));
	foreach($names as $name) {
		$clean = true;
		foreach(array_keys($words) as $word) {
			$pos = strpos( $name, $word.' ' );
			if ($pos!==false) {
				$clean = false;
			}
		}
		if ($clean) {
			$o[strtolower($name)] = 1;
		}
	}
	return array_keys($o);
  }
  function replace_accents($str) {
    $str = htmlentities($str, ENT_COMPAT, "UTF-8");
    $str = preg_replace(
      '/&([a-zA-Z])(uml|acute|grave|circ|tilde);/',
	  '$1',$str);
	return html_entity_decode($str);
  }

}
