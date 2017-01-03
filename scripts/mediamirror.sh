#!/bin/sh

#
### SF-Active Media Mirroring
### by: mtoups@indymedia.org
#

#  sf-active's article_class should call this script with one argument, the name/path of the file it wants to mirror.
#  some examples are given below.  rsync over ssh, with ssh public-key authentication would be the
#  preferred way to do this.  ftp is also possible -- an example is given with the ncftpput
#  command, and a file containing login info (less secure)
#
#  once you have this script working properly to upload your media,
#  you can change SF_UPLOAD_URL in sfactive.cfg to refer to your new
#  media server (or better yet, if you have more than one below DNS roundrobins!)

if test -f $1

then
# the echo lines below can be commented out if the message annoys you.
 echo "performing media mirror on $1...";
#   nohup rsync -a -e "ssh -2" $1 imc@hostanem.org:~/path/to/uploads/ &
#   nohup ncftpput -f /path/to/ftp-login-file /imc/testimc $1 &
   echo -e "done<br>\n";
fi

