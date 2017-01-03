
INSERT INTO category (name, order_num, category_id, template_name, creation_date, creator_id, default_feature_template_name,shortname,catclass,center,newswire,summarylength)  VALUES ( 'production', '0', '1', 'feature_list', '20011118142630', '0', '', 'production', 'm', 't', 's', '30');

#
# Dumping data for table 'event_topic'
#

INSERT INTO event_topic (event_topic_id, name) VALUES ( '0', 'other');

#
# Dumping data for table 'event_type'
#

INSERT INTO event_type (event_type_id, name) VALUES ( '0', 'other');

#
# Dumping data for table 'language'
#

INSERT INTO language (id, name, language_code) VALUES ('', 'default', '--');

#
# Dumping data for table 'location'
#

INSERT INTO location (location_id, name) VALUES ( '0', 'other');

#
# Dumping data for table 'user'
#

INSERT INTO user (user_id, username, password, first_name, last_name, email, phone) VALUES ( '1', 'username1', MD5('password'), '', '', '', '');


#
# Dumping data for table 'webcast'
#
