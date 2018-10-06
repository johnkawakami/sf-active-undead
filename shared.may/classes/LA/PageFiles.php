<?php  // vim:et:sw=4:ts=4:st=4:ai

namespace \LA\Traits;

/**
 * Helper to save an ArticleModel to files.
 *
 * This helps to save the article, not linked files.
 */
class PageFiles 
{
    private $article;
    private $id;
    private $filebasename;
    private $filepath;
    private $fileurl;
    private $articlepath;
    private $articleurl;

    function __construct(Article $article)
    {
        $this->article = $article;

        $year = $article->article["created_year"];
        $month = $article->article["created_month"];

        $this->articlepath =   SF_NEWS_PATH . "/$year/$month/";
        $this->articleurlpath =    SF_NEWS_URL . "/$year/$month/"
    }

    public function getHtmlPath() 
    {
        return $this->articlepath . '/' . $this->id . '.html';
    }

    public function getHtmlWithCommentsPath($text) 
    {
        return $this->articlepath . '/' . $this->id . '_comments.html';
    }

    public function getHtmlPhpPath($text) 
    {
        $filename = $this->articlepath . '/' . $this->id . '.php';
    }

    public function getHtmlPhpWithCommentsPath($text) 
    {
        $filename = $this->articlepath . '/' . $this->id . '_comments.php';
    }

    public function getJsonPath($text) 
    {
        $filename = $this->articlepath . '/' . $this->id . '.json';
    }

    public function getCanonicalUrl()
    {
    }

    public function assurePath($path) 
    {
    }

}


