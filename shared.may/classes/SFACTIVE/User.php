<?php // vim:et:ai:ts=4:sw=4

namespace SFACTIVE;

define('SALT','28ur8u0eiothioaoroerjsiofjdsfosjfefef');

class User
{
    //The user class represents a user in the system. For now all users
    //are admin users and can access any of the admin pages if they have a logon.
    //The user class contains all functions needed to add, update, delete users
    //as well as a function to authenticate users. 

    var $user_id;

    function get_user_id()
    {
        //returns the internal system id of the user
        return $this->user_id;
    }

    function set_user_id($new_user_id)
    {
        //sets the internal system if for a user
        $this->user_id=$new_user_is;
    }

    function get_user_fields($user_id)
    {
        //returns a dictionary object with all fields for a given user id
		$db_obj = new \SFACTIVE\DB;

        if (strlen($user_id) != 0)
        {
            $user_detail_query = "SELECT * FROM user WHERE user_id=".addslashes($user_id);
            $resultset = $db_obj->query($user_detail_query);
            $user = array_pop($resultset);
            return $user;
        }
        else
        {
            return 1;
        }
    }

    function get_list()
    {
        //returns an array of all users with all information for those users
        $db_obj= new \SFACTIVE\DB;
        $user_list_query = "SELECT * FROM user";
        $result = $db_obj->query($user_list_query);
        return $result;
    }

    function get_ordered_list($column = 'lastlogin', $style = '')
    {
	//returns an array of all users with all information for those users ordened on a certain column
	$db_obj= new \SFACTIVE\DB;
	if(!$style) $style = 'desc';
	if($column == "password"){ $column = 'user_id' ; }
	$user_list_query = "select * from user order by $column $style";
	$result = $db_obj->query($user_list_query);
	return $result;
    }

    function add($userfields)
    {
        //adds a user to the DB given a dictionary object containing the fields to use
        $db_obj= new \SFACTIVE\DB;

        if (strlen($userfields['username']) != 0 && strlen($userfields['password']) != 0)
        {
			$crypted = md5($userfields['password'].SALT);
            $user_add_query  = "INSERT INTO user (";
            $user_add_query .= "username, password, email, ";
            $user_add_query .= "phone, first_name, last_name";
            $user_add_query .= ") VALUES ('";
            $user_add_query .= $userfields['username']."',";
            $user_add_query .= "'$crypted', '";
            $user_add_query .= $userfields['email']."', '";
            $user_add_query .= $userfields['phone']."', '";
            $user_add_query .= $userfields['first_name']."', '";
            $user_add_query .= $userfields['last_name']."')";
	
            $error_num = $db_obj->execute($user_add_query);
        } else
        {
            $error_num = 1;
        }

        return $error_num;
    }

    function update($userfields)
    {
        //updates a user in the DB given a dictionary object containing the fields to use.
        //The id field used in the WHERE clause is inluded with the rest of the dictionary fields.

        $db_obj= new \SFACTIVE\DB;
        if (strlen($userfields['user_id']) != 0 && strlen($userfields['username']) != 0)
        {

            $user_update_query  = "UPDATE user SET ";
            $user_update_query .= "username='";
            $user_update_query .= $userfields['username'] . "',";

            if (strlen($userfields['user_id']) != 0 &&
                strlen($userfields['username']) != 0 &&
                strlen($userfields['password']) != 0)
            {
			    $crypted = md5($userfields['password'].SALT);
                $user_update_query .= "password='$crypted',";
            }
    
            $user_update_query .= "email='";
            $user_update_query .= $userfields['email'];
            $user_update_query .= "', phone='";
            $user_update_query .= $userfields['phone'];
            $user_update_query .= "', first_name='";
            $user_update_query .= $userfields['first_name'];
            $user_update_query .= "', last_name='";
            $user_update_query .= $userfields['last_name'];
            $user_update_query .= "' WHERE user_id=" . $userfields['user_id'];
            $error_num = $db_obj->execute($user_update_query);

        } else
        {
            $error_num = 1;
        }
        return $error_num;
    }
    
    function delete($user_id)
    {
        //Deletes a user from teh DB given a user id
        $db_obj= new \SFACTIVE\DB;
        if (strlen($user_id)!=0)
        {
            $user_delete_query = "DELETE FROM user WHERE user_id=" . $user_id;
            $error_num = $db_obj->execute($user_delete_query);
            $error_num=0;
        } else
        {
            $error_num = 1;
        }
        return $error_num;
    }

    function authenticate($username, $password)
    {
        //returns true if the username password combination is valid
        //otherwise returns false.
		if (!preg_match('/^[a-zA-Z0-9 -]+$/',$username)) die('bad username');

		$password = substr($password, 0, 50);  // trim long passwords 
		// set $md5_password to crypted when pws are changed
		$md5_password = md5($password);
		$salted = md5($password.SALT);

        $db_obj = new \SFACTIVE\DB();
        $user_authentication_query  = "SELECT username, user_id FROM user WHERE (username='";
        $user_authentication_query .= addslashes($username) . "' AND (password='" . $md5_password . "' OR password='$salted'))";

        $result = $db_obj->query($user_authentication_query);

        if (count($result) == 1)
        {
            $user_row = array_pop($result);
            $this->user_id = $user_row['user_id'];
            return true;
        } else
        {
            return false;
        }
    }

	function check_pass_security($password)
	{
	 if ($GLOBALS['admin_check_pass_security'] == 1) // only do this if set in the config
	 {
		if(strlen($password) < 6)
		{
			$returnmsg = "too_short";
		}else
		{
			if(preg_match("/[a-z]/", $password) && preg_match("/[A-Z]/", $password) && preg_match("/[0-9]/", $password))
			{
				$returnmsg = "ok" ;
			}else
			{
				$returnmsg = "insecure" ;
			}
		}
		return $returnmsg ;
	 }
	 else return "ok"; // if not configured to check, we always return ok
	}
	
}
