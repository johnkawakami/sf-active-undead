# Run this SQL to update your category table
# to allow for filtered newswires (local and 
# global in one wire).

alter table category modify column newswire enum('n','s','a','f');
