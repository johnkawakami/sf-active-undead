<?php  // vim:et:sw=4:ts=4:st=4:ai

namespace LA;

use \LA\ArticleGateway;
use \LA\Categories;

/**
 * This is not a MVC style model. It's a domain model.
 * We use ArticleGatewayAccelerated or ArticleGateway to access the table.
 */
class ArticleModel extends \Cache
{
    use Traits\DateMangler;

    protected $id = null;
    private $article = null;                 // An associative array of the article's data
    private $comments = null;                // A database resultset of the article's comments
    private $attachments = null;             // A database resultset of the article's attachments

    function __construct($article_id=null)
    {
        if ($article_id === null)
        {
            return $this;
        }

        $this->id = $article_id;
        $this->ag = new ArticleGateway('webcast');
        $this->article = $this->ag->find($article_id);
        if ($this->article !== null)
        {
            $this->comments = $this->ag->findComments($article_id);
            $this->attachments = $this->ag->findAttachments($article_id);
            $this->article['created_year'] = $this->article['created'];
            $this->article['created_month'] = $this->article['created'];
            $this->article['created_date'] = $this->article['created'];
            $this->canonicalUrl = SF_NEWS_URL.'/'.$this->article['created_year'].'/'.$this->article['created_month'].'/'.$this->article['id'].'.php';
        } 
        else
        {
            throw new \Exception('Could not find article');
        }
    }

    function updateNumcomments()
    {
        $this->ag->updateNumcomments($this->id);
    }

    function getCategories()
    {
        $cg = new CategoryGateway();
        $cats = $cg->findAllForArticle($this->id);
        return $cats;
    }
    
    function get_post_ids_starting_with ($start_id, $numtogen)
    {
        return $this->ag->findPostIdsStartingWith($start_id, $numtogen);
    }

}


