#! /bin/bash

# run this to rebuild the errors.tar file
# author: j@la.indymedia.org

cd /www/la.indymedia.org/public/errors
if test -e errors.tar ; then
    rm errors.tar
fi
cd /www/la.indymedia.org/
tar cf errors.tar public/errors/*
mv errors.tar public/errors

