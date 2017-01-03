#
# use this to update your pre Thu, 22 May 2003 sf-active database to get the categories working
# together with the new category code
#

alter table category add catclass enum ('m','t','l','h','i') after center;
alter table category add unique SHORTNAME_IDX (shortname);
alter table category modify shortname varchar(20) not null;
