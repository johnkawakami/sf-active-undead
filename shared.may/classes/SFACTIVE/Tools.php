<?php // vim:et:ai:ts=4:sw=4
namespace SFACTIVE;

class Tools 
{
    /**
     * Adds HTML links to bare URLs in the text.
     */
    static function linkBareUrls($data)
    {
        $data = preg_replace_callback('/(<a href=.+?<\/a>)/','self::guard_url',$data);
         
        $data = preg_replace_callback('/(https*:\/\/.+?)([ \n\r])/','self::link_url',$data);
        $data = preg_replace_callback('/^(https*:\/\/.+?)/','self::link_url',$data);
        $data = preg_replace_callback('/(https*:\/\/.+?)$/','self::link_url',$data);
                                  
        $data = preg_replace_callback('/{{([a-zA-Z0-9+=]+?)}}/','self::unguard_url',$data);
                                           
        return $data;
    }
                                                    
    static function guard_url($arr) { return '{{'.base64_encode($arr[1]).'}}'; }

    static function unguard_url($arr) { return base64_decode($arr[1]); }

    static function link_url($arr) { return self::guard_url(array('','<a href="'.$arr[1].'">'.$arr[1].'</a>')).$arr[2]; }

    static function saferHtml($text)
    {
        $text = strip_tags($text, '<p><b><i><u><ul><li><ol><table><tr><td><th><hr><strong><em><small><big><super><sub><br><div><span>');
        $text = str_replace(array('<?','?>','&quot;', '<%', '%>'),array('<!--','-->','"','&lt;%','%&gt;'),$text);
        $text = preg_replace('/(class|color|onblur|onchange|onclick|ondblclick|onfocus|onkeydown|onkeypress|onkeyup|onload|onmousedown|onmousemove|onmouseout|onmouseover|onmouseup|onreset|onselect|onsubmit|onunload)="[^"]+"/i', " ", $text);
        $text = preg_replace('/(class|color|onblur|onchange|onclick|ondblclick|onfocus|onkeydown|onkeypress|onkeyup|onload|onmousedown|onmousemove|onmouseout|onmouseover|onmouseup|onreset|onselect|onsubmit|onunload)=[^\>]+\>/i', ">", $text);
        return $text;
    }

    static function cleanupText($tmpvar)
    {
        // Process text articles, adds url's, etc
        $tmpvar = stripslashes($tmpvar);

        // replace newlines with html
        $tmpvar = preg_replace("/(\n\n|\r\r|\r\n\r\n|\n\t|\r\t\|\r\n\t)/i", " <br /><br /> ", $tmpvar);
        $tmpvar = preg_replace("/(\n|\r|\r\n)/i", " <br /> ", $tmpvar);

        // turn URLs into links;
        $tmpvar = self::linkBareUrls($tmpvar);

        return $tmpvar;
    }

    function cleanupHtml ($tmpvar)
    {
        return self::saferHtml($tmpvar);
        $tmpvar = str_replace('<title>','<!-- ',$tmpvar);
        $tmpvar = str_replace('</title>',' -->',$tmpvar);
        return $tmpvar;
    }



}
