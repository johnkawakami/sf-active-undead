#! /bin/bash

	composite -dissolve 50% -gravity SouthEast -quality 90 imc-watermark.png $1 tmp.jpg
	mv tmp.jpg $1
