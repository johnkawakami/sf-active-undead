#
# use this to change your live dbase when you install the new files for the calendar admin 
# creation date: 12 august 2003
# most recent update: 20 august -- should be ok now.
#
# usage: mysql -u your_user_name -p your_db_name < patch_calendar_for_install.sql
#


alter table location add UNIQUE KEY LOCATION_ID_IDX (location_id) ;
alter table event_topic add UNIQUE KEY EVENT_TOPIC_ID_IDX (event_topic_id) ;
alter table event_type add UNIQUE KEY EVENT_TYPE_ID_IDX (event_type_id) ;

