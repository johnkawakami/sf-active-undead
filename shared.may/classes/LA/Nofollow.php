<?php // vim:et:ai:ts=4:sw=4
namespace LA;

class Nofollow 
{

    /**
     * Rewrite all the a hrefs to be nofollow.
     * see - https://stackoverflow.com/questions/5037592/how-to-add-rel-nofollow-to-links-with-preg-replace
     * Note - this may break if the mime type is text/html. It could probably use Tidy to clean up
     * the HTML first.
     */
    static function forceNofollow($text) {
        if ($text=='') { return $text; }

        $dom = new \DOMDocument();
        $dom->preserveWhitespace = FALSE;
        $last_error_level = \error_reporting(E_ERROR|E_PARSE);
        $dom->loadHTML('<?xml encoding="UTF-8">'.$text);
        \error_reporting($last_error_level);
        foreach($dom->getElementsByTagName('a') as $a) {
            $nofollow = $dom->createTextNode('nofollow');
            $rel = $a->attributes->getNamedItem('rel');
            if ($rel) {
                if ($rel->nodeValue=='nofollow') {
                    // do nothing
                } else {
                    $rel->appendChild($nofollow);
                }
            } else {
                $newrel = $dom->createAttribute('rel');
                $newrel->appendChild($nofollow);
                $a->appendChild($newrel);
            }
        }
        return self::trimHtmlWrapper($dom->saveHTML());
    }
    static function removeNofollow($text) {
        $dom = new \DOMDOcument();
        $dom->preserveWhitespace = FALSE;
        $dom->loadHTML($text);
        foreach($dom->getElementsByTagName('a') as $a) {
            $rel = $a->attributes->getNamedItem('rel');
            if ($rel) {
                if ($rel->nodeValue=='nofollow') {
                    $a->removeAttribute('rel');
                }
            } 
        }
        return self::trimHtmlWrapper($dom->saveHTML());
    }
    private static function trimHtmlWrapper($text) {
        $text = preg_replace('/<\!DOCTYPE.+?<body>/s', '', $text);
        $text = preg_replace('/<\/body><\/html>/', '', $text);
        return $text;
    }

}
