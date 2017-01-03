#! /bin/bash

for fn in $@
do
	composite -dissolve 30% -gravity SouthEast -quality 90 imc-watermark.png public/uploads/$fn tmp.jpg
	echo $fn
	mv tmp.jpg public/uploads/$fn
done
