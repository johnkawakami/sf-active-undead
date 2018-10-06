<?php

/* this class deals with image compression/resizing issues and thumbnailing
 *
 * -- st3
 */

include_once("shared/global.cfg");

class Image
{

    function Image ()
    {
		// Nothing
    }

	function validate_image ($image_address,$image_ext)	// Return 1 for validated
	{
	echo "validate_image($image_address, $image_ext)<br>";
	list($ow,$oh) = getimagesize($image_address);
	
	if ($GLOBALS['max_image_x'] && $GLOBALS['max_image_y']) {

		$ow_g=$GLOBALS['max_image_x']-$ow;
		$oh_g=$GLOBALS['max_image_y']-$oh;
		//print($ow_g);
		//print($oh_g);
		if (($ow_g < $GLOBALS['max_image_x']/100*$GLOBALS['max_image_x_tolerance']) || ($oh_g < $GLOBALS['max_image_y']/100*$GLOBALS['max_image_y_tolerance'])) {
			$image_set=1;
			switch ($image_ext) {
				case 'jpg':
				case 'jpeg':
				case 'jpe':
					$im=imagecreatefromjpeg($image_address);
					break;
				case 'png':
					$im=imagecreatefrompng($image_address);
					break;
				case 'gif':
					$im=imagecreatefromgif($image_address);
					break;
			}
			if (!$im) print('could not create image from '.$image_address);
			if ($ow_g < $oh_g) {
				//print('first_case');
				$imn=imagecreatetruecolor($GLOBALS['max_image_x'],intval($GLOBALS['max_image_x']*$oh/$ow));
				if (!$imn) print('could not create new image');
				imagecopyresized($imn,$im,0,0,0,0,$GLOBALS['max_image_x'],intval($GLOBALS['max_image_x']*$oh/$ow),$ow,$oh);
			} else {
				$imn=imagecreatetruecolor(intval($GLOBALS['max_image_y']*$ow/$oh),$GLOBALS['max_image_y']);
				if (!$imn) print('could not create new image');
				//print('second_case');
				imagecopyresized($imn,$im,0,0,0,0,intval($GLOBALS['max_image_y']*$ow/$oh),$GLOBALS['max_image_y'],$ow,$oh);
			}
			switch ($image_ext) {
				case 'jpg':
				case 'jpeg':
				case 'jpe':
					if (imagejpeg($imn,$image_address.'mid.'.$image_ext))
						print(' reduced jpeg written for '.$image_address.'<br>');
					else
						print (' failed to write reduced jpeg for '.$image_address.'<br>');
					break;
				case 'gif':
				case 'png':
					imagepng($imn,$image_address.'mid.'.$image_ext);
					break;
			}
	
//print_r(getimagesize($image_address));
//print_r(getimagesize($image_address.'mid'));
	
//		imagedestroy($im);
		imagedestroy($imn);
	
		}	
	}

	if ($GLOBALS['image_thumbnail_x'] && $GLOBALS['image_thumbnail_y']) {
	
		$ow_g=$GLOBALS['image_thumbnail_x']-$ow;
		$oh_g=$GLOBALS['image_thumnail_y']-$oh;
//		print($ow_g);
//		print($oh_g);
		if (($ow_g < 0) || ($oh_g < 0)) {
			if (!$image_set) {
				switch ($image_ext) {
					case 'jpg':
					case 'jpeg':
					case 'jpe':
						$im=imagecreatefromjpeg($image_address);
						break;
					case 'png':
						$im=imagecreatefrompng($image_address);
						break;
					case 'gif':
						$im=imagecreatefromgif($image_address);
						break;
				}
				$image_set=1;
			}
			if ($ow_g < $oh_g) {
//				print('first_case');
				$imn=imagecreatetruecolor($GLOBALS['image_thumbnail_x'],intval($GLOBALS['image_thumbnail_x']*$oh/$ow));
				imagecopyresized($imn,$im,0,0,0,0,$GLOBALS['image_thumbnail_x'],intval($GLOBALS['image_thumbnail_x']*$oh/$ow),$ow,$oh);
			} else {
				$imn=imagecreatetruecolor(intval($GLOBALS['image_thumbnail_y']*$ow/$oh),$GLOBALS['image_thumbnail_y']);
//				print('THIRD_case');
				imagecopyresized($imn,$im,0,0,0,0,intval($GLOBALS['image_thumbnail_y']*$ow/$oh),$GLOBALS['image_thumbnail_y'],$ow,$oh);
			}
			switch ($image_ext) {
				case 'jpg':
				case 'jpeg':
				case 'jpe':
					//print('jpeg_written');
					imagejpeg($imn,$image_address.'thumb.'.$image_ext);
					break;
				case 'gif':
				case 'png':
					imagepng($imn,$image_address.'thumb.'.$image_ext);
					break;
			}
	
	
//			imagedestroy($im);
			imagedestroy($imn);
		
		}	
	}

	if ($GLOBALS['max_image_size'] || $_POST['max_image_size']) {

		if (file_exists($image_address.'mid')) $image_address .= 'mid';

		if ( !isset($_POST['image_compression_level']) || ($_POST['image_compression_level']<1) ) $_POST['image_compression_level'] = $GLOBALS['image_compression_level'];

		if  (filesize($image_address) > $GLOBALS['max_image_size']) { //|| (filesize($image_address) > $_POST['max_image_size']) ) {
			if(!$image_set) {
				switch ($image_ext) {
					case 'jpg':
					case 'jpeg':
					case 'jpe':
						$im=imagecreatefromjpeg($image_address);
						break;
					case 'png':
						$im=imagecreatefrompng($image_address);
						break;
					case 'gif':
						$im=imagecreatefromgif($image_address);
						break;
				}
				$image_set=1;
				}
			switch ($image_ext) {
				case 'gif':				
					$i=1;
					$e=imagecolorstotal($im)/$_POST['image_compression_level'];
					while ($i<$e) $i=$i*2;
				case 'png':
					if (!$i) $i=256;					
					imagetruecolortopalette($im,1,$i);
					if (function_exists('imagepng')) {	
						imagepng($im,$image_address.'mid.'.$image_ext);
						break;
					}
				case 'jpg':
				case 'jpeg':
				case 'jpe':
					if (function_exists('imagejpeg')) {
						imagejpeg($im,$image_address.'mid.'.$image_ext,100-$_POST['image_compression_level']*18);
					}
					break;	
			}

		}

	}
	
	if ($image_set) imagedestroy($im);
	return 1;

	}

	function change_size_from_old ($image_address, $year, $month) {

	eregi("([a-z0-9/\._-]+)\.([a-z0-9]+)$", $image_address, $regs);
	$upload_ext = $regs[2];
	
	$orig_image_address=$image_address;

	while (file_exists($image_address.'mid.'.$upload_ext) || file_exists($image_address.'thumb.'.$upload_ext))
			 {
			      $tmpname=tempnam(SF_UPLOAD_PATH. "/" . $year . "/" . $month , basename($image_address));
			      unlink($tmpname);
			      $tmpname=strtolower($tmpname);
			      $image_address = $tmpname.".".$upload_ext;
			      $filenamechanged=1;
			 }


	if ($filenamechanged) rename($orig_image_address,$image_address);
	list($ow,$oh) = getimagesize($image_address);
	
	if ($GLOBALS['max_image_x'] && $GLOBALS['max_image_y']) {

		$ow_g=$GLOBALS['max_image_x']-$ow;
		$oh_g=$GLOBALS['max_image_y']-$oh;
		//print($ow_g);
		//print($oh_g);
//		if (($ow_g < 0) || ($oh_g < 0)) {

		if (($ow_g < $GLOBALS['max_image_x']/100*$GLOBALS['max_image_x_tolerance']) || ($oh_g < $GLOBALS['max_image_y']/100*$GLOBALS['max_image_y_tolerance'])) {

			$image_set=1;
			switch ($upload_ext) {
				case 'jpg':
				case 'jpeg':
				case 'jpe':
					$im=imagecreatefromjpeg($image_address);
					break;
				case 'png':
					$im=imagecreatefrompng($image_address);
					break;
				case 'gif':
					$im=imagecreatefromgif($image_address);
					break;
			}
			if ($ow_g < $oh_g) {
				//print('first_case');
				$imn=imagecreatetruecolor($GLOBALS['max_image_x'],intval($GLOBALS['max_image_x']*$oh/$ow));
				imagecopyresized($imn,$im,0,0,0,0,$GLOBALS['max_image_x'],intval($GLOBALS['max_image_x']*$oh/$ow),$ow,$oh);
			} else {
				$imn=imagecreatetruecolor(intval($GLOBALS['max_image_y']*$ow/$oh),$GLOBALS['max_image_y']);
				//print('second_case');
				imagecopyresized($imn,$im,0,0,0,0,intval($GLOBALS['max_image_y']*$ow/$oh),$GLOBALS['max_image_y'],$ow,$oh);
			}
			switch ($upload_ext) {
				case 'jpg':
				case 'jpeg':
				case 'jpe':
					//print('jpeg_written');
					imagejpeg($imn,$image_address.'mid.'.$upload_ext);
					break;
				case 'gif':
				case 'png':
					imagepng($imn,$image_address.'mid.'.$upload_ext);
					break;
			}
	
	
//		imagedestroy($im);
		imagedestroy($imn);
	
		}	
	}

	if ($GLOBALS['image_thumbnail_x'] && $GLOBALS['image_thumbnail_y']) {
	
		$ow_g=$GLOBALS['image_thumbnail_x']-$ow;
		$oh_g=$GLOBALS['image_thumnail_y']-$oh;
		//print($ow_g);
		//print($oh_g);
		if (($ow_g < 0) || ($oh_g < 0)) {
			if (!$image_set) {
				switch ($upload_ext) {
					case 'jpg':
					case 'jpeg':
					case 'jpe':
						$im=imagecreatefromjpeg($image_address);
						break;
					case 'png':
						$im=imagecreatefrompng($image_address);
						break;
					case 'gif':
						$im=imagecreatefromgif($image_address);
						break;
				}
				$image_set=1;
			}
			if ($ow_g < $oh_g) {
				$imn=imagecreatetruecolor($GLOBALS['image_thumbnail_x'],intval($GLOBALS['image_thumbnail_x']*$oh/$ow));
				imagecopyresized($imn,$im,0,0,0,0,$GLOBALS['image_thumbnail_x'],intval($GLOBALS['image_thumbnail_x']*$oh/$ow),$ow,$oh);
			} else {
				$imn=imagecreatetruecolor(intval($GLOBALS['image_thumbnail_y']*$ow/$oh),$GLOBALS['image_thumbnail_y']);
				imagecopyresized($imn,$im,0,0,0,0,intval($GLOBALS['image_thumbnail_y']*$ow/$oh),$GLOBALS['image_thumbnail_y'],$ow,$oh);
			}
			switch ($upload_ext) {
				case 'jpg':
				case 'jpeg':
				case 'jpe':
					//print('jpeg_written');
					imagejpeg($imn,$image_address.'thumb.'.$upload_ext);
					break;
				case 'gif':
				case 'png':
					imagepng($imn,$image_address.'thumb.'.$upload_ext);
					break;
			}
	
	
//			imagedestroy($im);
			imagedestroy($imn);
		
		}	
	}

	if ($GLOBALS['max_image_size'] || $_POST['max_image_size']) {

		if (file_exists($image_address.'mid.'.$upload_ext)) $image_address_suffix = 'mid.'.$upload_ext;

		if ( !isset($_POST['image_compression_level']) || ($_POST['image_compression_level']<1) ) $_POST['image_compression_level'] = $GLOBALS['image_compression_level'];

		if ( (filesize($image_address) > $GLOBALS['max_image_size']) || (filesize($image_address) > $_POST['max_image_size']) ) {
				switch($upload_ext){
					case 'jpg':
					case 'jpeg':
					case 'jpe':
						$im=imagecreatefromjpeg($image_address.$image_address_suffix);
						break;
					case 'png':
						$im=imagecreatefrompng($image_address.$image_address_suffix);
						break;
					case 'gif':
						$im=imagecreatefromgif($image_address.$image_address_suffix);
						break;
				$image_set=1;
			}
			switch ($upload_ext) {
				case 'gif':				
					$i=1;
					$e=imagecolorstotal($im)/$_POST['image_compression_level'];
					while ($i<$e) $i=$i*2;
				case 'png':
					if (!$i) $i=256;					
					imagetruecolortopalette($im,1,$i);
					if (function_exists('imagepng')) {	
						imagepng($im,$image_address.$image_address_suffix);
						break;
					}
				case 'jpg':
				case 'jpeg':
				case 'jpe':
					if (function_exists('imagejpeg')) {
						imagejpeg($im,$image_address.$image_address_suffix,100-$_POST['image_compression_level']*18);
					}
					break;	
			}

		}

	}
	
	if ($image_set) imagedestroy($im);
	if ($filenamechanged) return $image_address; else return 0;

	}

}
