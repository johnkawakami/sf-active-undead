select concat('wget ', IMAGE_URL)
   from FEATURES
   where SITE_ID=5693 and IMAGE_URL<>'';
