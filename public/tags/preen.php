<?php

/**
 sketch for a preening script

 copy all parent article titles and summaries over into a local cache of titles
 and use this table instead of webcast
 
 copy all IDs of hidden articles to a local temp table

 purging all hidden articles from the article_tags, and also from the cache

 removing duplicate articles in the article_tags lists

 for all ignored tags, find the articles, and purge from the cache

 Look for duplicate titles and save these to a list of potential duplicate articles

 remove all php and html and json files for hidden articles (in news)

 Scan logs for 404 errors, and put them into a table for analysis.

 Write a sitemap.
