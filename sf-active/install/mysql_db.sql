# --------------------------------------------------------
#
# Table structure for table 'category'
#

DROP TABLE IF EXISTS category;
CREATE TABLE category (
   name varchar(200),
   order_num int(5),
   category_id int(11) NOT NULL auto_increment,
   template_name varchar(200) DEFAULT '0' NOT NULL,
   creation_date timestamp(14),
   creator_id int(8) DEFAULT '0' NOT NULL,
   default_feature_template_name varchar(200) DEFAULT '0' NOT NULL,
   shortname varchar(20) NOT NULL,
   summarylength int(5),
   parentid int(11),
   newswire enum('n','s','a','f'),
   center enum('t','f'),
   catclass enum ('m','t','l','h','i', 'e', 'p', 'o'),
   description text,
   PRIMARY KEY (category_id),
   UNIQUE KEY SHORTNAME_IDX (shortname)
);

# --------------------------------------------------------
#
# Table structure for table 'event'
#

DROP TABLE IF EXISTS event;
CREATE TABLE event (
   event_id int(11) NOT NULL auto_increment,
   start_date datetime,
   end_date datetime,
   duration int(11),
   location_id int(11),
   location_other varchar(50),
   event_topic_id int(11),
   event_topic_other varchar(50),
   event_type_id int(11),
   title varchar(50),
   location_details text,
   contact_name varchar(50),
   contact_phone varchar(50),
   contact_email varchar(50),
   confirmation_number int(11),
   description text,
   event_type_other varchar(50),
   linked_file varchar(160),
   mime_type varchar(50),
   event varchar(5),
   language_id tinyint unsigned,
   artmime enum('h', 't'),
   PRIMARY KEY (event_id)
);


# --------------------------------------------------------
#
# Table structure for table 'event_topic'
#

DROP TABLE IF EXISTS event_topic;
CREATE TABLE event_topic (
   event_topic_id int(11),
   name varchar(50),
   UNIQUE KEY EVENT_TOPIC_ID_IDX (event_topic_id)
);


# --------------------------------------------------------
#
# Table structure for table 'event_type'
#

DROP TABLE IF EXISTS event_type;
CREATE TABLE event_type (
   event_type_id int(11),
   name varchar(50),
   UNIQUE KEY EVENT_TOPIC_ID_IDX (event_type_id)
);


# --------------------------------------------------------
#
# Table structure for table 'feature'
#

DROP TABLE IF EXISTS feature;
CREATE TABLE feature (
   feature_version_id int(11) NOT NULL auto_increment,
   feature_id int(11) DEFAULT '0' NOT NULL,
   summary blob,
   title1 varchar(200),
   title2 varchar(200),
   display_date varchar(100),
   order_num int(5),
   category_id int(5),
   template_name varchar(200) DEFAULT '0' NOT NULL,
   modification_date timestamp(14),
   creation_date timestamp(14),
   creator_id int(8) DEFAULT '0' NOT NULL,
   status char(2) DEFAULT 'c',
   tag varchar(100),
   image varchar(100),
   version_num int(5) DEFAULT '1',
   is_current_version int(1) DEFAULT '1',
   modifier_id int(11),
   image_link varchar(200),
   language_id tinyint unsigned,
   PRIMARY KEY (feature_version_id)
);


# --------------------------------------------------------
#
# Table structure for table 'feature_dossier'
#

DROP TABLE IF EXISTS feature_dossier;
CREATE TABLE feature_dossier (
  id_dossier int(11) NOT NULL auto_increment,
  id_feature int(11) default NULL,
  name varchar(255) default NULL,
  cat_id int(11) default NULL,
  PRIMARY KEY  (id_dossier)
) TYPE=MyISAM;

# --------------------------------------------------------
#
# Table structure for table 'feature_sequence'
#

DROP TABLE IF EXISTS feature_sequence;
CREATE TABLE feature_sequence (
   feature_id int(10) NOT NULL auto_increment,
   PRIMARY KEY (feature_id)
);

#--------------------------------------------------------
#
# Table structure for table `language`
#
DROP TABLE IF EXISTS language;
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


# --------------------------------------------------------
#
# Table structure for table 'location'
#

DROP TABLE IF EXISTS location;
CREATE TABLE location (
   location_id int(11), 
   name varchar(50),
   UNIQUE KEY LOCATION_ID_IDX (location_id)
);


# --------------------------------------------------------
#
# Table structure for table 'role'
#

DROP TABLE IF EXISTS role;
CREATE TABLE role (
   role_id int(8) NOT NULL auto_increment,
   name varchar(50) NOT NULL,
   description blob NOT NULL,
   PRIMARY KEY (role_id)
);


# --------------------------------------------------------
#
# Table structure for table 'template'
#

DROP TABLE IF EXISTS template;
CREATE TABLE template (
   template_id int(8) NOT NULL auto_increment,
   name varchar(50) NOT NULL,
   type varchar(8) NOT NULL,
   text blob NOT NULL,
   creation_date timestamp(14),
   creator_id int(8),
   version int(8) DEFAULT '0' NOT NULL,
   status int(2) DEFAULT '0' NOT NULL,
   PRIMARY KEY (template_id)
);


# --------------------------------------------------------
#
# Table structure for table 'user'
#

DROP TABLE IF EXISTS user;
CREATE TABLE user (
   user_id int(11) NOT NULL auto_increment,
   username varchar(100) NOT NULL,
   password varchar(100) NOT NULL,
   first_name varchar(100) NOT NULL,
   last_name varchar(100) NOT NULL,
   email varchar(100) NOT NULL,
   phone varchar(100) NOT NULL,
   lastlogin datetime,
   PRIMARY KEY (user_id)
);


# --------------------------------------------------------
#
# Table structure for table 'user_role'
#

DROP TABLE IF EXISTS user_role;
CREATE TABLE user_role (
   user_id int(8) DEFAULT '0' NOT NULL,
   role_id int(8) DEFAULT '0' NOT NULL
);

# --------------------------------------------------------
#
# Table structure for table 'validation'
#

DROP TABLE IF EXISTS validation;
CREATE TABLE validation (
  article_id int(11) NOT NULL,
  validated enum('f','t'),
  hash char(32),
  PRIMARY KEY (article_id)
);


# --------------------------------------------------------
#
# Table structure for table 'webcast'
#

DROP TABLE IF EXISTS webcast;
CREATE TABLE webcast (
   id int(11) NOT NULL auto_increment,
   heading varchar(90),
   author varchar(45),
   article blob,
   contact varchar(80),
   link varchar(160),
   address varchar(160),
   phone varchar(80),
   parent_id int(10) DEFAULT '0' NOT NULL,
   mime_type varchar(50),
   summary text,
   numcomment int(5),
   arttype varchar(50),
   html_file varchar(160),
   modified timestamp(14),
   created timestamp(14),
   linked_file varchar(160),
   mirrored enum('f','t'),
   display enum('f','t','l','g'),
   rating decimal(9,2),
   artmime enum('h','t'),
   language_id tinyint unsigned,
   PRIMARY KEY (id)
);

# --------------------------------------------------------
#
# Table structure for table 'webcast_dossier'
#

DROP TABLE IF EXISTS webcast_dossier;
CREATE TABLE webcast_dossier (
  id int(11) NOT NULL auto_increment,
  webcast_id int(11) default NULL,
  id_dossier int(11) default NULL,
  parent_id int(11) default NULL,
  order_num int(11) default NULL,
  nest_level int(11) default NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;



# --------------------------------------------------------
#
# Table structure for table 'weblink'
#

DROP TABLE IF EXISTS weblink;
CREATE TABLE weblink (
   fromlink varchar(10),
   tolink varchar(10)
);

# --------------------------------------------------------
#
# Table structure for table 'catlink'
#

DROP TABLE IF EXISTS catlink;
create table catlink (
   catid bigint(21),
   id int(10)
);
