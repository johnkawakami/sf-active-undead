<?php // vim:et:ai:ts=4:sw=4
namespace LA;

class Mangle 
{

    function render_file_size ($path)
    {
        // Returns HTML for a filesize

        $size = filesize($path);
        if ($size < 1048576)
        {
            $size = $size / 1024;
            $size = sprintf("%.1f",$size);
            $size = $size . " kibibytes";
        } else
        {
            $size = $size / 1048576;
            $size = sprintf("%.1f",$size);
            $size = $size . " mebibytes";
        }

        return $size;
    }

    function escape_quotes_newlines($text) {
        return str_replace(array('"',"'","\n","\r",'\\'),array('&quot;',"&apos;",' ','','\\\\'), $text);
    }

    function cleanup_text($tmpvar)
    {
        // Process text articles, adds url's, etc
        $tmpvar = stripslashes($tmpvar);

        // replace newlines with html
        $tmpvar = preg_replace("/(\r\r \r\r|\n\n \n\n|\r\n\r\n \r\n\r\n|\n\n|\r\r|\r\n\r\n|\n\t|\r\t\|\r\n\t)/", " <br /><br /> ", $tmpvar);
        $tmpvar = preg_replace("/(\n|\r|\r\n)/", " <br /> ", $tmpvar);

        return $tmpvar;
    }

    function link_text2($tmpvar)
    {
        // A less confusing version of link_text1.

        // guard all <a link text:
        $tmpvar = preg_replace_callback('/<a(.+?)<\\/a>/', function ($t) {
                    return ":--@".base64_encode($t[1])."@--:";
                }, $tmpvar);
        if ($tmpvar == NULL) return "";
        $tmpvar = preg_replace('!(http://|https://)([a-zA-Z0-9@:%_.~#\?&/=-]+[a-zA-Z0-9@:%_~#\?&/=-])!i', '<a href="$1$2" rel="nofollow">$1$2</a>', $tmpvar);
        $tmpvar = preg_replace('/[A-Za-z0-9_]([-._]?[A-Za-z0-9])*@[A-Za-z0-9]([-.]?[A-Za-z0-9])*\.[A-Za-z]+/i', '<a href="mailto:\\0">\\0</a>', $tmpvar);
        $tmpvar = preg_replace_callback('/:--@(.+?)@--:/', function ($t) {
                 $dec = base64_decode($t[1]); return "<a$dec</a>";
             }, $tmpvar);
        return $tmpvar;
    }

    function link_text($tmpvar) {
        return Nofollow::forceNofollow($this->link_text2($tmpvar));
    }

    function damage_links($text)
    {
        $text = preg_replace_callback('/\\s\\.+?.com/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/\\s\\.+?.org/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/www\\.+?\\s/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/www\\.+?$/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/www\\.+?[>.]/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/https{0,1}:\\/\\/.+?\\s/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/https{0,1}:\\/\\/.+?$/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        $text = preg_replace_callback('/https{0,1}:\\/\\/.+?[>.]/i',create_function('$matches','return "base64:".base64_encode($matches[0]);'),$text); 
        return $text;
    }

    function cleanup_html($text) 
    {
        $text = strip_tags($text, '<p><b><i><u><ul><li><ol><table><tr><td><th><hr><strong><em><small><big><super><sub><br><div><span>');
        $text = str_replace(array('<?','?>','&quot;', '<%', '%>'),array('<!--','-->','"','&lt;%','%&gt;'),$text);
        $text = preg_replace('/(class|color|onblur|onchange|onclick|ondblclick|onfocus|onkeydown|onkeypress|onkeyup|onload|onmousedown|onmousemove|onmouseout|onmouseover|onmouseup|onreset|onselect|onsubmit|onunload)="[^"]+"/i', " ", $text);
        $text = preg_replace('/(class|color|onblur|onchange|onclick|ondblclick|onfocus|onkeydown|onkeypress|onkeyup|onload|onmousedown|onmousemove|onmouseout|onmouseover|onmouseup|onreset|onselect|onsubmit|onunload)=[^\>]+\>/i', ">", $text);
        return $text;
    }

}
