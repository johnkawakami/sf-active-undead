<?php

class CSRF {
    function make_token($name) {
        $db_obj = new DB();
        $tokens = $this->generate_tokens($name);
        $sql = 'INSERT INTO csrf (`key`) VALUES (\''.$tokens[0].'\')';
        $db_obj->execute_statement($sql);
        $sql = 'INSERT INTO csrf (`key`) VALUES (\''.$tokens[1].'\')';
        $db_obj->execute_statement($sql);   
        return $tokens[0];
    }
    function generate_tokens($name) {
        $salt = 'fjqf2398rhwilajrhq289ry2qry239f84yhq3ryoq8dfuiw35yh83f7qgror23r';
        $date = new DateTime();
        $oneday = new DateInterval('P1D');
        $token = array();
        $token[0] = md5($_SERVER['REMOTE_ADDR'].$salt.$name.$date->format('d'));
        $token[1] = md5($_SERVER['REMOTE_ADDR'].$salt.$name.$date->add($oneday)->format('d'));
        return $token;
    }
    function validate_token($token) {
        // if (!$token) return false;
        $db_obj = new DB();
        $sql = "SELECT COUNT(*) ct FROM csrf WHERE `key`='$token'";
        $result = $db_obj->query($sql);
        if ((int)$result[0]['ct'] > 0) {
            $sql = "DELETE FROM csrf WHERE key='$token'";
            $db_obj->execute_statement($sql);
            return true;
        }
        return false;
    }
}
