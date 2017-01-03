<?php

// converts articles to UTF-8

$byte_map=array();
init_byte_map();
$ascii_char='[\x00-\x7F]';
$cont_byte='[\x80-\xBF]';
$utf8_2='[\xC0-\xDF]'.$cont_byte;
$utf8_3='[\xE0-\xEF]'.$cont_byte.'{2}';
$utf8_4='[\xF0-\xF7]'.$cont_byte.'{3}';
$utf8_5='[\xF8-\xFB]'.$cont_byte.'{4}';
$nibble_good_chars = "@^($ascii_char+|$utf8_2|$utf8_3|$utf8_4|$utf8_5)(.*)$@s";

// loop over the old table

// pull the article content

// is it latin-1?

  // convert to utf-8
  // insert into new table

// loop over the new table

// is it latin-1

  // convert it to utf-8
  // update the new table

function connect_dbs() {
	global $olddb;
	global $newdb;
	$olddb = new PDO('mysql:host=localhost;dbname=laindy',
               'root', 'a$pir3ate');
	$newdb = new PDO('mysql:host=localhost;dbname=la_indymedia_org;charset=utf8',
			   'root', 'a$pir3ate');
	// that charset parameter matters.
}
connect_dbs();

$log = fopen('update-log.txt','w');

for($i=225000;$i<300000;$i+=1000) {
		$offset = $i;
		$count = 1000;
		$sth = $olddb->prepare("SELECT id,article,heading,author,summary FROM webcast LIMIT $offset,$count");
		if (!$sth) {
			fwrite( $log, "olddb->prepare failed in $count,$offset\n" );
			connect_dbs();
			continue;
		}
		$sth->execute();

		while ($res = $sth->fetch()) {
			$id = $res['id'];
			foreach(array('article','heading','author','summary') as $field) {
				if (!detectASCII($res[$field]) and !detectUTF8($res[$field])) {
					$value = fix_latin($res[$field]);
					$check = $newdb->prepare("SELECT $field FROM webcast WHERE id=$id");
					if (!$check) {
						fwrite( $log, "newdb->prepare failed in $count,$offset\n" );
						connect_dbs();
						continue;
					}
					$check->execute();
					$res = $check->fetch();
					if ($res[$field] != $value) {
						$upd = $newdb->prepare("UPDATE LOW_PRIORITY webcast SET $field=? WHERE id=?");
						if (!$upd) {
							fwrite( $log, "newdb->prepare failed in $count,$offset\n" );
							connect_dbs();
							continue;
						}
						$upd->execute( array( $value, $id ) );
						fwrite( $log, "$offset,$count updated $id field $field\n");
					} else {
						fwrite( $log, "$offset,$count skipping $id field $field\n");
					}
				}
			}  
		}
}
fclose($log);

//----------------------------------------------------functions

function detectUTF8($string)
{
        return preg_match('%(?:
        [\xC2-\xDF][\x80-\xBF]        # non-overlong 2-byte
        |\xE0[\xA0-\xBF][\x80-\xBF]               # excluding overlongs
        |[\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}      # straight 3-byte
        |\xED[\x80-\x9F][\x80-\xBF]               # excluding surrogates
        |\xF0[\x90-\xBF][\x80-\xBF]{2}    # planes 1-3
        |[\xF1-\xF3][\x80-\xBF]{3}                  # planes 4-15
        |\xF4[\x80-\x8F][\x80-\xBF]{2}    # plane 16
        )+%xs', $string);
}

function detectCP1252($str)
{
    // see http://goodtools.net/pages/HowCharactersMap.html
    // Windows CP 1252 range is 128-159
    return (preg_match('/[\x80-\x9F]/', $str) === 1);
}
function detectISO88591($str)
{
    // the iso8859-1 ranges are 160-255
    return (preg_match('/[\xA0-\xFF]/', $str) === 1);
} 
function detectASCII($str)
{
    // true if it's 7-bit ascii throughout
    return (preg_match("/^[\\x00-\\x7F]*$/", $str) === 1);
}

function init_byte_map(){
  global $byte_map;
  for($x=128;$x<256;++$x){
    $byte_map[chr($x)]=utf8_encode(chr($x));
  }
  $cp1252_map=array(
    "\x80"=>"\xE2\x82\xAC",    // EURO SIGN
    "\x82" => "\xE2\x80\x9A",  // SINGLE LOW-9 QUOTATION MARK
    "\x83" => "\xC6\x92",      // LATIN SMALL LETTER F WITH HOOK
    "\x84" => "\xE2\x80\x9E",  // DOUBLE LOW-9 QUOTATION MARK
    "\x85" => "\xE2\x80\xA6",  // HORIZONTAL ELLIPSIS
    "\x86" => "\xE2\x80\xA0",  // DAGGER
    "\x87" => "\xE2\x80\xA1",  // DOUBLE DAGGER
    "\x88" => "\xCB\x86",      // MODIFIER LETTER CIRCUMFLEX ACCENT
    "\x89" => "\xE2\x80\xB0",  // PER MILLE SIGN
    "\x8A" => "\xC5\xA0",      // LATIN CAPITAL LETTER S WITH CARON
    "\x8B" => "\xE2\x80\xB9",  // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
    "\x8C" => "\xC5\x92",      // LATIN CAPITAL LIGATURE OE
    "\x8E" => "\xC5\xBD",      // LATIN CAPITAL LETTER Z WITH CARON
    "\x91" => "\xE2\x80\x98",  // LEFT SINGLE QUOTATION MARK
    "\x92" => "\xE2\x80\x99",  // RIGHT SINGLE QUOTATION MARK
    "\x93" => "\xE2\x80\x9C",  // LEFT DOUBLE QUOTATION MARK
    "\x94" => "\xE2\x80\x9D",  // RIGHT DOUBLE QUOTATION MARK
    "\x95" => "\xE2\x80\xA2",  // BULLET
    "\x96" => "\xE2\x80\x93",  // EN DASH
    "\x97" => "\xE2\x80\x94",  // EM DASH
    "\x98" => "\xCB\x9C",      // SMALL TILDE
    "\x99" => "\xE2\x84\xA2",  // TRADE MARK SIGN
    "\x9A" => "\xC5\xA1",      // LATIN SMALL LETTER S WITH CARON
    "\x9B" => "\xE2\x80\xBA",  // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
    "\x9C" => "\xC5\x93",      // LATIN SMALL LIGATURE OE
    "\x9E" => "\xC5\xBE",      // LATIN SMALL LETTER Z WITH CARON
    "\x9F" => "\xC5\xB8"       // LATIN CAPITAL LETTER Y WITH DIAERESIS
  );
  foreach($cp1252_map as $k=>$v){
    $byte_map[$k]=$v;
  }
}

function fix_latin($instr){
  if(mb_check_encoding($instr,'UTF-8'))return $instr; // no need for the rest if it's all valid UTF-8 already
  global $nibble_good_chars,$byte_map;
  $outstr='';
  $char='';
  $rest='';
  while((strlen($instr))>0){
    if(1==preg_match($nibble_good_chars,$instr,$match)){
      $char=$match[1];
      $rest=$match[2];
      $outstr.=$char;
    }elseif(1==preg_match('@^(.)(.*)$@s',$instr,$match)){
      $char=$match[1];
      $rest=$match[2];
      $outstr.=$byte_map[$char];
    }
    $instr=$rest;
  }
  return $outstr;
}

// writes out a utf-8 html file to the utf8 directory
function emit_html_file($s,$name) {
  if (''==$s) {
    //fwrite($fh,$s);
    $name = '';
  }
  else if (detectASCII($s)) {
    //fwrite($fh,$s);
    $name = '';
  }
  else if (detectCP1252($s)) {
    $o = fix_latin($s);
    $name = preg_replace('/utf8\//','utf8/CP1252-',$name);
  }
  else if (detectISO88591($s)) {
    $o = fix_latin($s);
    $name = preg_replace('/utf8\//','utf8/ISO88591-',$name);
  }
  else if (detectUTF8($s)) {
    //fwrite($fh,$s);
    $name = '';
  }
  if ($name!='') {
    $fh = fopen($name,'w');
    fwrite($fh,"<!DOCTYPE html>\n<html><head><meta charset='UTF-8' /></head>");
    fwrite($fh,"<body>");
    fwrite($fh,$o);
    fwrite($fh,"</body>");
    fwrite($fh,"</html>");
    fclose($fh);
  }
}
