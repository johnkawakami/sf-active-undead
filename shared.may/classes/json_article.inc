<?php

/**
 * Alters the article so some fields are not included.
 * Also eventually will scan the filesystem to look for
 * thumbnails, and add those into a field named image.
 */
function extract_json_article($a) {
	$out = array();
	$fields = array( 'id', 'heading', 'author', 'created', 'modified',
		'last_modified', 'article', 'contact', 'link', 'address',
		'phone', 'parent_id', 'mime_type', 'summary', 'numcomment',
		'fileurl', 'filesize', 'article_url');
	foreach($fields as $field) {
		$out[$field] = $a[$field];
	}
	if (preg_match('/^image\\//',$a['mime_type'])) {
		$out['image'] = find_thumbs($a['fileurl']);
	}
	return $out;
}

/**
 * Normalizes the comment and fixes up the image parts.
 */
function extract_json_comments($a) {
	$out = array();
	$fields = array( 'id', 'heading', 'article', 'author', 'parent_id',
		'mime_type', 'created', 'modified', 'linked_file');
	foreach($a as $element) {
		$comment = array();
		foreach($fields as $field) {
			$comment[$field] = $element[$field];
		}
		$comment['fileurl'] = str_replace(SF_UPLOAD_PATH, 'http://la.indymedia.org/uploads', $comment['linked_file']);
		if (preg_match('/^image\\//',$comment['mime_type'])) {
			$comment['image'] = find_thumbs($comment['fileurl']);
		}
		unset($comment['linked_file']);
		$out[] = $comment;
	}
	return $out;
}

/**
 * Given a url, it finds smaller versions and returns
 * an array of these urls.
 */
function find_thumbs( $url ) {
	$path = SF_UPLOAD_PATH.str_replace('http://la.indymedia.org/uploads','',$url);
	$thumbs = array('original'=>$url);
	if (file_exists($path.'thumb.jpg')) {
		$thumbs['small'] = $url.'thumb.jpg';
	}
	if (file_exists($path.'mid.jpg')) {
		$thumbs['medium'] = $url.'mid.jpg';
	}
	return $thumbs;
}
