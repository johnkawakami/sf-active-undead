<?php // vim:et:ai:ts=4:sw=4
include('shared/vendor/autoload.php');

include_once("shared/global.cfg");
include_once(SF_CLASS_PATH."/spamc_class.inc");
define("TAGS_CACHE_PATH", SF_CACHE_PATH.'/tags/');

function print_hash($hash) 
{
  echo "<table>";
  foreach($hash as $k=>$v)
    echo "<tr><td>$k</td><td>$v</td></tr>";
  echo "</table>";
}

function build_tags_list($article) {
  global $db;
  $id = $article->article['id'];
  $text .= $article->article['heading']. '  ';
  $text .= $article->article['summary'] . '  ';
  $text .= $article->article['article'];
  $s = new SpamC(strip_tags($text));
  foreach($s->keywords as $word=>$count) {
  	$density = $s->keyword_density[$word];
	insert_tag_article($word,$id,$density);
  }
  foreach($s->names as $name) {
  	insert_tag_article($name,$id,0.01);
  }
}
function insert_tag_article($word,$article_id,$density) {
	global $db;
	$tag_id = $db->queryFetchOne("SELECT id FROM tags WHERE name=:word", [':word'=>$word] );
	if ($tag_id===NULL) {
		$db->insert("INSERT INTO tags (name) VALUES (:word)", [':word'=>$word] );
		$tags = $db->query("SELECT id FROM tags WHERE name=:word", [':word'=>$word] );
	}
	$db->insert("INSERT INTO tags_articles (tag_id,article_id,density) VALUES (:tag_id,:article_id,:density)", 
		    [':tag_id'=>$tag_id, ':article_id'=>$article_id, ':density'=>$density] );
}

function get_tags_for_article($id) {
  global $db;
  if (!preg_match('/^[0-9]{1,7}$/', $id)) die();
$tags = $db->query("SELECT tag_id AS id, tags.name AS tag, tags.ignore AS ig, tags.synonym AS syn FROM tags_articles JOIN tags ON tag_id=tags.id WHERE article_id = $id AND tags.ignore<>1");

  $o =array();
  // create a dictionary of tags
  foreach($tags as $tag) {
  	$t[$tag['tag']] = $tag['id'];
  }
  // fold synonyms
  foreach($tags as $tag) {
  	if ($tag['syn']!='') {
		if(isset($t[$tag['syn']])) {
			// the term is already in the result
		} else {
			$tag = get_tag_by_name($tag['syn']);
			$o[] = $tag;
			$t[$tag['tag']] = $tag['id'];
		}
	} else {
		// add this tag to the result
		$o[] = $tag;
	}
  }
  return $o;
}
function get_tag_by_name($name) {
  global $db;
  if (!preg_match('/^[a-z0-9 ]+$/i', $name)) die(); //matches only alphanumerics and space
  $tag = $db->query("SELECT id, tags.name AS tag, tags.ignore AS ig, tags.synonym AS syn FROM tags WHERE tags.name='$name'");
	if (count($tag)==0) {
	    $db->insert("INSERT INTO tags (name) VALUES ('$name')");
        $tag = $db->query("SELECT id, tags.name AS tag, tags.ignore AS ig, tags.synonym AS syn FROM tags WHERE tags.name='$name'");
	}
	return $tag[0];
}
