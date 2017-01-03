#!/bin/bash


if [ -z "$1" ]; then
	echo -e "Welcome to sfa-perm script\n\n  This script will set your permissions right.\n use it in your sitename dir\n\n usage: ./sfa-perm.sh username user_of_webserver\n eg: ./sfa-perm.sh sfactive apache\n\n	NOTE: this is only for new installs !\n\n"
		exit
fi

if [ -z "$2" ]; then
	echo -e "you need to specify the user your webserver runs as (usually www, www-data or apache)"
	exit
fi

user=$1
webserver=$2

chown -R $user.$user local scripts website
find local scripts website -type d -exec chmod 775 {} \;
find local scripts website -type f -exec chmod 664 {} \;
chown $webserver local/include local/include/*.inc local/include/*.php local/templates/ local/templates/*.tpl local/cache local/logs/ local/logs/sf* local/cache/pages/ local/cache/pages/*.inc local/cache/calendar/ local/cache/calendar/event* local/cache/*.html local/cache/hashes_time local/cache/*.txt local/cache/*.inc local/cache/galleries/
chown $webserver website/news website/newswire.* website/syn/ website/themes/ website/themes/*.css website/process website/uploads/ website/features/ website/calendar/event_cache website/calendar/week_cache/ website/archives/cache/ website/im/ website/images/ website/imcenter website/im/*.png website/im/*.jpg website/im/*.gif website/images/*.gif website/images/*.jpg website/images/*.png 
