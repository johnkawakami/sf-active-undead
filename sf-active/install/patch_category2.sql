#
# use this to update your pre 16 April 2004 but later then Thu, 22 May 2003 sf-active database to get the categories working
# together with the new category code
#

# if your dbase doesn't have the catclass, first do patch_category_for_old_db.sql

alter table category change catclass catclass enum ('m','t','l','h','i', 'e', 'p', 'o');
