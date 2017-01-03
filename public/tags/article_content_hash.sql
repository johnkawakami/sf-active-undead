create table article_content_hash (
article_id int(11),
content_hash varchar(128),
primary key (article_id),
index (content_hash)
)
