<?php


// Class for the Content Header Local Page
class content_header extends LocalPage {

    function content_header($settings = null) {
	if ($settings==null) $settings = array();
	// fixme - validate names
	// $valid = array('noindex');
	$this->settings = $settings;
        return 1;
    }

    function execute () {

		// Translate all TPL_ in the Template
		$tr = new Translate();
		$tr->create_translate_table('content_header');
		$this->translation($tr);

		// this kind of code and other stuff is what should be moved out per occam's idea
		$headerdate = strtolower(date("l, F j, Y"));

		// Theme
		$url_string = "";
		foreach ($_GET as $key => $value)
		{
		    if ($key != "theme")
		    {
		        $url_string .= "&" . $key . "=" . $value;
		    }
		}		
		$this->tkeys['URL_STRING'] = $url_string;

		// Page Title
		if ( $GLOBALS['page_title'] )
			{ 
				$this->tkeys['HEAD_TITLE'] = $GLOBALS['page_title'] . " : ".$GLOBALS['site_nick']; 
			} else { 
				$this->tkeys['HEAD_TITLE'] = $GLOBALS['site_name']; 
			} 

		// Robot Meta Tag
		if ( ($GLOBALS['page_display']=='f') or ($_GET['display']=='f') or ($this->settings['noindex']) 
		{ 
			$this->tkeys['META_ROBOT'] = '<meta name="ROBOTS" content="NOINDEX, NOFOLLOW" />';
		} else {
			$this->tkeys['META_ROBOT'] = "";
		}

		// Canonical URL
		if ($this->settings['canonical']) {
			$this->tkeys['CANONICAL'] = '<link rel="canonical" href="'.$this->settings['canonical'].'" />';
		} else {
			$this->tkeys['CANONICAL'] = '';
		}

		
		// Theme Definitions - keep it simple and stupid
		$this->tkeys['CSS_THEME'] = $GLOBALS['sf_theme'];
		$this->tkeys['SITE_NAME'] = $GLOBALS['site_name'];
		switch ($GLOBALS['sf_theme'])
		{
			case "theme1.css":
				$this->tkeys['IMAGE_SRC_INDY'] = "/images/sfindyblack.gif"; 
				$this->tkeys['IMAGE_WIDTH_INDY'] = "148"; 
				$this->tkeys['IMAGE_SRC_BANNER'] = "/images/imcsfobb16.gif"; 
				$this->tkeys['IMAGE_WIDTH_BANNER'] = "418"; 
				break;				

			case "theme4.css":
				$this->tkeys['IMAGE_SRC_INDY'] = "/images/sfindyblack.gif"; 
				$this->tkeys['IMAGE_WIDTH_INDY'] = "148"; 
				$this->tkeys['IMAGE_SRC_BANNER'] = "/images/logo.jpg"; 
				$this->tkeys['IMAGE_WIDTH_BANNER'] = "418"; 
				break;				

			case "theme6.css":
				$this->tkeys['IMAGE_SRC_INDY'] = "/images/sfbayarea.gif"; 
				$this->tkeys['IMAGE_WIDTH_INDY'] =  "245"; 
				$this->tkeys['IMAGE_SRC_BANNER'] = "/images/logo.jpg"; 
				$this->tkeys['IMAGE_WIDTH_BANNER'] = "418"; 
				break;				

			case "theme7.css":
				$this->tkeys['IMAGE_SRC_INDY'] = "/im/bump.gif"; 
				$this->tkeys['IMAGE_WIDTH_INDY'] =  "1"; 
				$this->tkeys['IMAGE_SRC_BANNER'] = "/images/imc_banner.gif"; 
				$this->tkeys['IMAGE_WIDTH_BANNER'] = "468"; 
				break;				

			case "theme8.css":
				$this->tkeys['IMAGE_SRC_INDY'] = "/im/bump.gif"; 
				$this->tkeys['IMAGE_WIDTH_INDY'] =  "1"; 
				$this->tkeys['IMAGE_SRC_BANNER'] = "/images/imc_banner2.gif"; 
				$this->tkeys['IMAGE_WIDTH_BANNER'] = "468"; 
				break;				

			case "theme9.css":
				$this->tkeys['IMAGE_SRC_INDY'] = "/images/sfindyblack.gif"; 
				$this->tkeys['IMAGE_WIDTH_INDY'] = "148"; 
				$this->tkeys['IMAGE_SRC_BANNER'] = "/images/blood.gif"; 
				$this->tkeys['IMAGE_WIDTH_BANNER'] = "418"; 
				break;

			default:
				$this->tkeys['IMAGE_SRC_INDY'] = "/images/sfindy.gif";
				$this->tkeys['IMAGE_WIDTH_INDY'] = "148"; 
				$this->tkeys['IMAGE_SRC_BANNER'] = "/images/logo.jpg"; 
				$this->tkeys['IMAGE_WIDTH_BANNER'] = "418"; 
				break;
		}

		if ($GLOBALS['left'] == "noleft") 
		{ 
			$this->tkeys['NO_LEFT'] = ''.			
'					</table>'.
'				</td>'.
'			</tr>'.
'		</table>';

		} else { 
			$this->tkeys['NO_LEFT'] = ""; 
		}

	}

	function translation ($tr) {		
		$this->tkeys['LANG_CONTACT_US'] = 	$GLOBALS['dict']['contact_us'];
		$this->tkeys['LANG_SUBSCRIBE_ME'] = $GLOBALS['dict']['subscribe_me'];
		$this->tkeys['LANG_CALENDAR'] = 	$GLOBALS['dict']['calendar'];
		$this->tkeys['LANG_ABOUT_US'] = 	$GLOBALS['dict']['about_us'];
		$this->tkeys['LANG_PUBLISH'] = 		$GLOBALS['dict']['publish'];

	}
	
}
