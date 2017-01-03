# the table user now stores passwords encrypted.
# if you upgrade from an old install, you'll need to update your dbase
#
# usage: mysql -u your_user_name -p your_db_name < patch_user_passwords.sql

update user set password = md5(password) ;
