CREATE TABLE feature_dossier (
  id_dossier int(11) NOT NULL auto_increment,
  id_feature int(11) default NULL,
  name varchar(255) default NULL,
  cat_id int(11) default NULL,
  PRIMARY KEY  (id_dossier)
) TYPE=MyISAM;

CREATE TABLE webcast_dossier (
  id int(11) NOT NULL auto_increment,
  webcast_id int(11) default NULL,
  id_dossier int(11) default NULL,
  parent_id int(11) default NULL,
  order_num int(11) default NULL,
  nest_level int(11) default NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

