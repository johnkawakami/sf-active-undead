#
# use this to update your pre 22 april 2004 database so that email_validation is possible.
#

alter table user add lastlogin datetime;
