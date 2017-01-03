CREATE TABLE language (
  id tinyint unsigned NOT NULL auto_increment,
  name varchar(25) default NULL,
  language_code varchar(5) NOT NULL default '',
  order_num int(5) default NULL,
  display enum('t','f') default 't',
  build enum('y','n') default 'n',
  PRIMARY KEY  (id),
  UNIQUE KEY LANGUAGE_CODE_IDX (language_code)
) TYPE=MyISAM;
