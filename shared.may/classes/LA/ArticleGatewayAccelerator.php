<?php  // vim:ai:et:sw=4:ts=4:st=4:

namespace LA;

/**
 * This accelerator manages the webcast and it's partner tables, to
 * speed up inserts and reads.  It has the same interface as ArticleGateway,
 * but performs all the behind-the-scenes tricks to accerate the application.
 */

class ArticleGatewayAccelerator extends ArticleGateway
{
    function __construct( $ignored )
    {
        $this->webcast = new ArticleGateway('webcast');
        $this->webcastLatest = new ArticleGateway('webcast_latest');
        $this->webcastParents  = new ArticleGateway('webcast_parents');
        $this->pt = new ArticleGateway('webcast_parents_t');
        $this->pl = new ArticleGateway('webcast_parents_l');
    }

    function updateWebcastIndexes( $id, $display ) {
        $db_obj = new \SFACTIVE\DB;
        if ($display=='l') {
            $db_obj->execute("DELETE FROM webcast_parents_t WHERE id=$id");
            $db_obj->execute("INSERT INTO webcast_parents_l SELECT * FROM webcast WHERE id=$id");
            $db_obj->execute("INSERT INTO webcast_parents SELECT * FROM webcast WHERE id=$id 
                              ON DUPLICATE KEYS UPDATE webcast_parents.display=webcast.display");
        }
        if ($display=='t') {
            $db_obj->execute("DELETE FROM webcast_parents_l WHERE id=$id");
            $db_obj->execute("INSERT INTO webcast_parents_t SELECT * FROM webcast WHERE id=$id");
            $db_obj->execute("INSERT INTO webcast_parents SELECT * FROM webcast WHERE id=$id 
                              ON DUPLICATE KEYS UPDATE webcast_parents.display=webcast.display");
        }
        if ($display=='f') {
            $db_obj->execute("DELETE FROM webcast_parents_l WHERE id=$id");
            $db_obj->execute("DELETE FROM webcast_parents_t WHERE id=$id");
            $db_obj->execute("DELETE FROM webcast_parents WHERE id=$id");
        }
    }

    // fixme - appears this isn't used at all
    function dump_article($id) 
    {
        $id = filter_var( $id, FILTER_VALIDATE_INT );
        if (! $this->is_in_webcast_latest($id)) 
        {
            $db_obj = new DB;
            $db_obj->execute("INSERT INTO webcast_latest SELECT * FROM webcast WHERE id=$id");
            $db_obj->execute("INSERT INTO webcast_latest SELECT * FROM webcast WHERE parent_id=$id");
        }
    }

    function is_in_webcast_latest( $id ) 
    {
        $id = filter_var( (int) $id, FILTER_VALIDATE_INT );
        $db_obj = new \SFACTIVE\DB;
        $resultset = $db_obj->query("SELECT id FROM webcast_latest WHERE id=$id");
        return ($resultset[0]==$id);
    }

    function findComments($id)
    {
        if ($this->is_in_webcast_latest( $id )) {
            return $this->webcastLatest->findComments($id);
        } else {
            return $this->webcast->findComments($id);
        }
    }

    function findAttachments($id)
    {
        if ($this->is_in_webcast_latest( $this->article['id'] )) {
            return $this->webcastLatest->findAttachments($id);
        } else {
            return $this->webcast->findAttachments($id);
        }
    }

    function insert($row)
    {
        $id = $this->webcast->insert($row);
        $db_obj = new \SFACTIVE\DB;
        $db_obj->execute("START TRANSACTION");
        $db_obj->execute("DROP TABLE webcast_latest");
        $db_obj->execute("CREATE TABLE webcast_latest SELECT * FROM `webcast` ORDER BY id DESC LIMIT 1000");
        $db_obj->execute("COMMIT");
        $this->updateWebcastIndexes( $id, $row['display'] );
    }


    function update_article_status($id, $new_status)
    {
        $this->webcast->updateOneField($id, 'display', $new_status)
        $this->webcastLatest->updateOneField($id, 'display', $new_status)

        // fixme do this in the controller
        // $this->set_article_data($id);

        $this->updateWebcastIndexes( $id, $new_status );
    }

}


