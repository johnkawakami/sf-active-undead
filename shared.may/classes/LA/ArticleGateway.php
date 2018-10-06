<?php  // vim:ai:et:sw=4:ts=4:st=4:

namespace LA;

define('COMMENT_CACHE_LIMIT', 2048);

/**
 * A gateway to the webcast table, but with a parameter that lets
 * us specify a table. 
 *
 * The acceleration method is to work on a subset
 * of data, by copying a subset of data to smaller tables.  The 
 * constructor accepts a table name as a parameter. During runtime,
 * we should create gateways for each of the tables.
 */

class ArticleGateway
{
    protected $table; // table name
    protected $db; 

    function __construct($table, $pdodb)
    {
        $this->table = "`$table`";
        $this->db = $pdodb;
    }

    public function find($id) 
    {
        $t = $this->table;
        $stmt = $this->db->prepare("SELECT * FROM $t WHERE id=:id");
        $stmt->execute(['id'=>$id]);
        return $stmt->fetchArray(PDO::FETCH_ASSOC);
    }
    
    public function update($row)
    {
        $t = $this->table;
        $stmt = $this->db->prepare(
        "UPDATE $t
            SET heading=:heading, 
                author=:author,
                article=:article,
                contact=:contact,
                link=:link,
                mime_type=:mime_type,
                artmime=:artmime,
                summary=:summary
                address=:address
                display=:display
                arttype=:arttype
                phone=:phone
                parent_id=:parent_id
                linked_file=:linked_file
                validate=:validate
                language_id=:language_id
             WHERE id=:id");

        $data = \LA\Tools::arrayToPDOParameters($row);
        $data[':modified'] = now();
        $stmt->execute($data);
    }

    public function insert($row)
    {
        $t = $this->table;
        $stmt = $this->db->prepare(
        "INSERT INTO $t 
            (heading, author, contact, link, address, 
             phone, mime_type, summary, numcomment, arttype, artmime, html_file, mirrored, 
             display, linked_file, created, modified, id, article, parent_id, rating, language_id)
             VALUES 
            (:heading, :author, :contact, :link, :address, 
             :phone, :mime_type, :summary, :numcomment, :arttype, :artmime, :html_file, :mirrored, 
             :display, :linked_file, :created, :modified, :id, :article, :parent_id, :rating, :language_id)
             ");
        $data = \LA\Tools::arrayToPDOParameters($row);
        $data[':created'] = $data[':modified'] = now();
        $stmt->execute($data);
        $id = $this->db->lastInsertId();
        return $id;
    }

    public function updateNumcomments($id)
    {
        // the accelerator overrides this anyway, so leaving it out
    }
    /**
     * @returns int number of non-hidden comments for post $id.
     */
    function getNumcomments($id)
    {
        $t = $this->table;
        $this->db->prepare("SELECT COUNT(*) FROM $t WHERE parent_id=:id AND display<>'h' AND arttype<>'attachment'");
    }

    /*
     * Returns an array of all the comments, hidden and visible, in order from oldest to newest.
     *
     * The original function had sorting, limiting, and filtering, but PHP has array filters that
     * can do this in code. Comment lists tend to be < 10 long, and almost all are < 100 long, so 
     * doing this in code is fine. Examples:
     *
     * $filtered = array_filter($comments, function ($row) { return 'f' != $row['display']; });
     *
     * uasort($comment, function($a, $b) { return ($a['created'] < $b['created']) ? -1 : 1; } );
     */
    public function findComments($id)
    {
        $t = $this->table;
        $stmt = $this->db->prepare("SELECT * FROM $t WHERE parent_id=:id AND arttype='comment' ORDER BY created ASC");
    }

    public function findAttachments($id)
    {
        $stmt = $this->db->prepare("SELECT * FROM $t WHERE parent_id=:id AND arttype='attachment' ORDER BY created ASC");
    }

    /**
     * @return array Int list of IDs of posts where id > $start_id
     */
    function findPostIdsStartingWith($start_id, $numtogen)
    {
        // Process article id's for use with the regeneration code
        $t = $this->table;
        $this->db->prepare("SELECT id FROM $t where parent_id=0 and id>=:start ORDER BY id LIMIT 0, :count");
        $stmt->execute([':start' => $start_id, ':count' => $numtogen]);
        return $stmt->fetchAll( PDO::FETCH_COLUMN, 0 );
    }

    /**
     * Generalized update to set a single value in a single row.
     *
     * // update article status
     * $tbgw->updateOneField( $id, 'display', $status );
     */
    function updateOneField( $id, $field, $value ) 
    {
        $t = $this->table;
        $field = "`$field`";
        $stmt = $this->db->prepare("UPDATE $t SET $field=:value WHERE id=:id");
        $stmt->execute([ 
            ':id' => $id, 
            ':value' => $value
        ]);
    }
}


