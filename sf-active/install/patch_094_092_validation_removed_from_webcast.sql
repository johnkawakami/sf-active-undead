# use this patch if you used patch_email_validation for version 092
# now we store the validation info in its own table, not in webcast

INSERT INTO validation SELECT id as article_id, 't' as validated FROM webcast WHERE validate = 'validated';

INSERT INTO validation SELECT id as article_id, 'f' as validated, RIGHT(validate,32) as hash FROM webcast WHERE validate like '%hash%';

# say bye bye to that old text column

ALTER TABLE webcast DROP COLUMN validate;
