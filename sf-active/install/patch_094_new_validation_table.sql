# validation has its own table now 

CREATE TABLE validation (
  article_id int(11) NOT NULL,
  validated enum('f','t'),
  hash varchar(32),
  PRIMARY KEY (article_id)
);
