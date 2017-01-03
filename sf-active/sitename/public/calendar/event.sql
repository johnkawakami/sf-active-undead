# phpMyAdmin MySQL-Dump
# version 2.2.0
# http://phpwizard.net/phpMyAdmin/
# http://phpmyadmin.sourceforge.net/ (download page)
#
# Host: localhost
# Generation Time: September 8, 2001, 4:28 pm
# Server version: 3.23.26
# PHP Version: 4.0.4pl1
# Database : `indymedia`
# --------------------------------------------------------

#
# Table structure for table `event`
#

DROP TABLE IF EXISTS event;
CREATE TABLE event (
  event_id int(11) default NULL auto_increment,
  title varchar(80) NOT NULL default '                                                                                ',
  start_date datetime NOT NULL default '0000-00-00 00:00:00',
  duration int(11) default NULL,
  location_id int(11) default NULL,
  location_other varchar(60) default NULL,
  location_details text,
  event_type_id int(11) default NULL,
  event_type_other varchar(60) default NULL,
  event_topic_id int(11) default NULL,
  event_topic_other varchar(60) default NULL,
  contact_name varchar(60) default NULL,
  contact_email varchar(60) default NULL,
  contact_phone varchar(60) default NULL,
  description longtext,
  created_date timestamp(14) default NULL,
  created_by tinyint(4) default NULL,
  PRIMARY KEY (event_id)
) TYPE=MyISAM;

#
# Dumping data for table `event`
#

# --------------------------------------------------------

#
# Table structure for table `event_topic`
#

DROP TABLE IF EXISTS event_topic;
CREATE TABLE event_topic (
  event_topic_id int(11) NOT NULL default '0',
  name varchar(40) default NULL,
  PRIMARY KEY (event_topic_id)
) TYPE=MyISAM;

#
# Dumping data for table `event_topic`
#

INSERT INTO event_topic (event_topic_id, name) VALUES ( '0', 'other');
INSERT INTO event_topic (event_topic_id, name) VALUES ( '1', 'labor');
INSERT INTO event_topic (event_topic_id, name) VALUES ( '2', 'environment');
INSERT INTO event_topic (event_topic_id, name) VALUES ( '4', 'globalization');
INSERT INTO event_topic (event_topic_id, name) VALUES ( '6', 'militarization');
INSERT INTO event_topic (event_topic_id, name) VALUES ( '7', 'free speach');
INSERT INTO event_topic (event_topic_id, name) VALUES ( '8', 'civil rights/human rights');
# --------------------------------------------------------

#
# Table structure for table `event_type`
#

DROP TABLE IF EXISTS event_type;
CREATE TABLE event_type (
  event_type_id int(11) NOT NULL default '0',
  name varchar(40) default NULL,
  PRIMARY KEY (event_type_id)
) TYPE=MyISAM;

#
# Dumping data for table `event_type`
#

INSERT INTO event_type (event_type_id, name) VALUES ( '1', 'SF IMC  meeting');
INSERT INTO event_type (event_type_id, name) VALUES ( '2', 'non-imc meeting');
INSERT INTO event_type (event_type_id, name) VALUES ( '3', 'large protest/rally (1000+ people)');
INSERT INTO event_type (event_type_id, name) VALUES ( '4', 'medium sized protest (100-1000 people)');
INSERT INTO event_type (event_type_id, name) VALUES ( '5', 'small protest/press conference');
INSERT INTO event_type (event_type_id, name) VALUES ( '6', 'small protest/action');
INSERT INTO event_type (event_type_id, name) VALUES ( '0', 'other event type');
INSERT INTO event_type (event_type_id, name) VALUES ( '7', 'film/video event');
INSERT INTO event_type (event_type_id, name) VALUES ( '8', 'concert');
# --------------------------------------------------------

#
# Table structure for table `location`
#

DROP TABLE IF EXISTS location;
CREATE TABLE location (
  location_id int(11) NOT NULL default '0',
  name varchar(40) default NULL,
  description text,
  url varchar(100) default NULL,
  PRIMARY KEY (location_id)
) TYPE=MyISAM;

#
# Dumping data for table `location`
#

INSERT INTO location (location_id, name, description, url) VALUES ( '0', 'other', '', '');
INSERT INTO location (location_id, name, description, url) VALUES ( '1', 'San Francisco / Downtown', '', '');
INSERT INTO location (location_id, name, description, url) VALUES ( '2', 'San Francisco / Mission District', '', '');
INSERT INTO location (location_id, name, description, url) VALUES ( '3', 'East Bay / Berkeley', '', '');
INSERT INTO location (location_id, name, description, url) VALUES ( '4', 'East Bay / Oakland', '', '');
INSERT INTO location (location_id, name, description, url) VALUES ( '6', 'San Francisco / Other', '', '');
INSERT INTO location (location_id, name, description, url) VALUES ( '7', 'East Bay / Other', '', '');
INSERT INTO location (location_id, name, description, url) VALUES ( '8', 'South Bay', '', '');
INSERT INTO location (location_id, name, description, url) VALUES ( '9', 'North Bay', '', '');
INSERT INTO location (location_id, name, description, url) VALUES ( '10', 'Northern California', '', '');
INSERT INTO location (location_id, name, description, url) VALUES ( '11', 'Southern California', '', '');

