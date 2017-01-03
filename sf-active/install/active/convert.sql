drop table feature ;
drop table feature_sequence ;

CREATE TABLE feature (
  feature_version_id int(11) NOT NULL auto_increment,
  feature_id int(11) NOT NULL default '0',
  summary blob,
  title1 varchar(200) default NULL,
  title2 varchar(200) default NULL,
  display_date varchar(100) default NULL,
  order_num int(5) default NULL,
  category_id int(5) default NULL,
  template_name varchar(200) NOT NULL default '0',
  creation_date timestamp(14) NOT NULL,
  creator_id int(8) NOT NULL default '0',
  status char(2) default 'c',
  tag varchar(100) default NULL,
  image varchar(100) default NULL,
  version_num int(5) default '1',
  is_current_version int(1) default '1',
  modification_date timestamp(14) NOT NULL,
  modifier_id int(11) default NULL,
  image_link varchar(200) default NULL,
  PRIMARY KEY  (feature_version_id),
  KEY feature_pk (feature_version_id),
  KEY feature_01 (order_num),
  KEY feature_02 (feature_id,is_current_version),
  KEY feature_03 (creation_date),
  KEY feature_04 (category_id,is_current_version,status),
  KEY feature_05 (category_id,status,is_current_version,creation_date),
  KEY feature_06 (creation_date,order_num),
  KEY feature_07 (feature_id),
  KEY feature_08 (version_num),
  KEY feature_09 (feature_id,version_num)
) TYPE=MyISAM;

CREATE TABLE feature_sequence (
  feature_id int(10) NOT NULL auto_increment,
  PRIMARY KEY  (feature_id)
) TYPE=MyISAM;


insert into feature (
  feature_version_id, 
  feature_id, 
  summary, 
  title1, 
  title2, 
  display_date,
  order_num, 
  category_id,
  template_name,
  creation_date,           
  creator_id,              
  status,                  
  tag,                     
  image,                   
  version_num,             
  is_current_version,      
  modification_Date,       
  modifier_id,             
  image_link
  ) 
  
select 
  ID,                                                    -- feature_version_id     
  ID,                                                    -- feature_id             
  concat(
    BODY,
    If(LINK_1<>'' || LINK_2<>'' || LINK_3<>'','<br> [ ',''),
    If(LINK_1<>'', concat('<a href="',LINK_1, '">', LINK_1_TITLE, '</a>'), ''),
    If(LINK_2<>'', concat(' | <a href="',LINK_2, '">', LINK_2_TITLE, '</a>'), ''),
    If(LINK_3<>'', concat(' | <a href="',LINK_3, '">', LINK_3_TITLE, '</a>'), ''),
    If(LINK_1<>'' || LINK_2<>'' || LINK_3<>'',']','')
  ),
                                                         -- summary                
  TITLE,                                                 -- title1                 
  SUB_TITLE,                                             -- title2                 
  date_format(TIMESTAMP, '%d/%m/%Y'),                     -- display_date           
  ID,                                                    -- order_num              
  '3',                                                   -- category_id            
  'italy_image_left',                                    -- template_name          
  TIMESTAMP,                                             -- creation_date          
  2,                -- creator_id                        -- creator_id             
  'a',                                                   -- status                 
  IMAGE_CAPTION,                                         -- tag                    
  IF(
     IMAGE_URL<>'',
     concat(
        '/images/',
        SUBSTRING(IMAGE_URL FROM LENGTH(IMAGE_URL)+2-LOCATE('/', REVERSE(IMAGE_URL)))
     ),
     ''
   ),	            -- needs to be converted             -- image                  
  1,                                                     -- version_num            
  1,                                                     -- is_current_version     
  CURRENT_TIMESTAMP,                                     -- modification_Date      
  2,                -- modified id                       -- modified_id            
  LINK_1                                                 -- image_link             

from features.FEATURES where SITE_ID = '5693' ;


insert into feature_sequence (feature_id) select ID from features.FEATURES where SITE_ID='5693';
