# eliminate "date_entered" from webcast, it is redundant given that
# we have "created"
# (don't do this until after you've upgraded to 094 first though)
#
# usage: mysql -u your_user_name -p your_db_name < patch_drop_date_entered.sql

ALTER TABLE webcast DROP COLUMN date_entered
