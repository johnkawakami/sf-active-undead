<?php
// vim:ts=4:sw=4:ai

/* this class deals with image compression/resizing issues and thumbnailing
 * -- st3
 *
 * Class rewritten with clearer logic and better variable names.
 */

include_once("shared/global.cfg");

class Image2
{
	var $path;
	var $ext;

    function __construct( $path=NULL )
    {
		if ($path) {
			$this->path = $path;
			$this->ext = pathinfo($path, PATHINFO_EXTENSION);
			$this->validate_image( $this->path, $this->ext );
			// all done :)
		}
    }

	function validate_image($image_address,$image_ext)	// Return 1 for validated
	{
		echo "validate_image($image_address, $image_ext)<br>";
		$this->targeted_resize( $image_address, $image_ext, 'mid', $GLOBALS['max_image_x'], $GLOBALS['max_image_y'] );
		$this->targeted_resize( $image_address, $image_ext, 'thumb', $GLOBALS['image_thumbnail_x'], $GLOBALS['image_thumnail_y'] );
		return 1;
	}
	/**
	 * This no longer handles name clashes gracefully.  It appears that the calling code
	 * starts off by renaming the file correctly, then calls this.  Should probably just
	 * get rid of this function and simply have the calling code call validate_image again.
	 */
	function change_size_from_old ($path, $year, $month) {
		echo "change_size_from_old( $path, $year, $month) <br>";
		$ext = pathinfo($path, PATHINFO_EXTENSION);
		$orig_path = $path;

		$tempname = tempnam( SF_UPLOAD_PATH.'/'.$year.'/'.$month, basename($path) );
		unlink($tempname);
		$tempname = strtolower($tempname);
		$path = $tempname . '.' .$ext;

		echo "$orig_path -> $path<br>";

		if ($orig_path != $path) { rename( $orig_path, $path ); }
		$mid = $path.'mid.'.$ext;
		$thumb = $path.'thumb.'.$ext;
		if (file_exists($mid)) {
			unlink($mid);
		} 
		if (file_exists($thumb)) {
			unlink($thumb);
		}
		$this->targeted_resize( $path, $ext, 'mid', $GLOBALS['max_image_x'], $GLOBALS['max_image_y'] );
		$this->targeted_resize( $path, $ext, 'thumb', $GLOBALS['image_thumbnail_x'], $GLOBALS['image_thumnail_y'] );

	  	if ($orig_path != $path) return $path;
		return 0; // else return 0 to indicate no name change
	}

	function targeted_resize( $path, $ext, $suffix, $maxw, $maxh=NULL ) 
	{
		if ($maxw==='' or $maxw===0) $maxw = NULL;
		if ($maxh==='' or $maxh===0) $maxh = NULL;
		// echo "targeted_resize( $path, $ext, $suffix, $maxw, $maxh )<br>";
		list($ow,$oh) = getimagesize($path);
		// echo "original dimensions wxh: $ow x $oh<br>";
		if (!$ow) die("could not get image size");
		$ratiow = $maxw / $ow;
		$ratioh = $maxh / $oh;
		if ($ow>$maxw or $oh>$maxh) {
			switch ($ext) {
				case 'jpg': case 'jpeg': case 'jpe':
					$orig = imagecreatefromjpeg($path);
					break;
				case 'png':
					$orig = imagecreatefrompng($path);
					break;
				case 'gif':
					$orig = imagecreatefromgif($path);
					break;
			}
			// logic is a little redundant...
			if ($maxw===NULL and $oh>$maxh) {
				// no width specified.  shrink width same as height.
				 // echo "maxw is not set<br>";
				$newh = $maxh;
				$neww = intval( $ow * $ratioh );
			} else if ( $maxh===NULL and $ow>$maxw) {
				// echo "maxh is not set<br>";
				$neww = $maxw;
				$newh = intval ( $oh * $ratiow );
			} else if ($ratiow < $ratioh) {
				// the original is too wide
				$neww = $maxw;
				$newh = intval ( $oh * $ratiow );
			} else {
				// the original is too tall
				$newh = $maxh;
				$neww = intval( $ow * $ratioh );
			}
			// echo "new wxh: $neww x $newh<br>";
			$new = imagecreatetruecolor( $neww, $newh );
			if (!$new) die("can't make image");

			imagecopyresized( $new, $orig, 0,0,0,0, $neww, $newh, $ow, $oh );
			$newpath = $path.$suffix;
			switch ($ext) {
				case 'jpg': case 'jpeg': case 'jpe':
					if (imagejpeg($new,$newpath))
						print(' reduced jpeg written for '.$newpath.'<br>');
					else
						print ('failed to write reduced jpeg for '.$newpath.'<br>');
					break;
				case 'gif':
				case 'png':
					imagepng( $new, $newpath );
					print(' reduced image written for '.$newpath.'<br>');
					break;
			}
			imagedestroy($orig);
			imagedestroy($new);
		}
	}
}
