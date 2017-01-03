# patch to allow to publish a file in the calendar
# mysql -u your_user_name -pyour_password your_db_name patch_094_calendar.sql

alter table event add column linked_file varchar(160) ;
alter table event add column mime_type varchar(50);

