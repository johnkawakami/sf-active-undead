#!/bin/env python2.2

##
## Python implementation of pushtoeye.
##
## mike warren (mike-warren.com) for IMC Alberta (running sf-active)
## with help from Jason Lanthier <cst00007 __at__ cs.camosun.bc.ca>
##
## thanks to toni, jb and agent humble for input at irc.indymedia.org, as well
## as shane and brandon from IMC Alberta.
##
## debugged for sf-active by mtoups -- october 2003

import os, os.path, re, sys, time, string, mimetypes, getopt, MySQLdb, StringIO

##
## options:
##
## besides changing the things below, the following command-line
## options work:
##
## --dummy  don't do anything (just print out what would have been done)
## --force  upload all files, even they're already on loudeye
## --tinyfiles  upload even files < 100 bytes
## --upload-only  don't change any .php files
## --stdout  log to stdout instead of the log file
## --verbose  verbose output
##

## defaults for command-line options

DUMMY = 0                               # if 1, don't do anything to loudeye
FORCE = 0                               # if 1, don't ignore files due to age or size
TINYFILES = 0                           # if 1, transfer files < 100 bytes
UPLOADONLY = 0                          # if 1, don't change .php files
STDOUT = 0                              # if 1, log to standard out
VERBOSE = 0				# if 1, be verbose

## things folks might want to change:
## (I kept the names close to what's in pushtoeye.pl)

CITYNAMES = ['sandboxtest']
## this needs exactly one %s directive, which will be the city-name
UPLOAD_TEMPLATE = '/www/sf-active/%s/website/uploads/'
CACHE_TEMPLATE = '/www/sf-active/%s/website/news/'
DATABASE_PASSWORD_RE = re.compile( '.*db_setup\\["password"\\] = "(.*)";' )

## localhost, unless you're using a remote db like we are -- matt
## (btw this might break if mysql isn't accepting tcp connections)
DBHOST = 'localhost'

## what are we looking to match when we replace those url's?
## some sites use like "indymediapr.org" instead of cityname.indymedia.org
FROB_URL_RE = re.compile(".*define\('SF_ROOT_URL',(.*)'http://(.*)'\);")

LOG_FILE = '/www/sf-active/logs/pushtoeye/pytoeye.log'
## contains plaintext ftp password -- make sure it isn't world-readable!
FTP_PASSWORD_FILE = '/www/sf-active/pypw'
FTP_USER = 'imc'
FTP_URL = 'images.indymedia.org'
PID_FILE = '/www/sf-active/logs/pushtoeye/pushtoeye.pid'

## lower-case list of all file-extensions which are desireable to
## mirror (files are found by both extension and mime-type)

MIRROR_EXTENSIONS = [ 'mp3', 'gif', 'jpg', 'jpeg', 'mpg', 'mpeg', 'rm', 'wma', 'wmv', 'avi', 'mov', 'ogg', 'pdf','mp4' ]
MIRROR_TYPES = [ 'audio/mpeg', 'audio/x-ogg', 'image/jpeg', 'video/mpeg', 'image/gif']

## map some file-names
RENAME_MAP = { 'wma':'asf',
               'wmv':'asf'}

## which extensions are streamable by loudeye (vs. static content like images
## or download-only content like mpegs). Don't worry about mp3's here, as they're
## handled specially: a .m3u file is auto-generated for each mp3.
STREAM_EXTENSIONS = [ 'wma', 'wmv', 'rm' ]
    
###
### below here be dragons
###
### that is, no more config-ish options to change
###

class Publisher:
    """
    Handles publishing of a file to loudeye, as well as
    the frobbing of the .php cache files. There wil be
    one instance of the appropriate class per session,
    so if you want to track state on a per-file basis,
    you'll also have to track the file...
    """

    def __init__( self ):
        pass


    def upload( self,
    		file,
                city,
                remotepath,
                connection ):
        """
        This should publish the File object file to
        loudeye; connection is an ftplib.FTP object.
        """
        logger.log( "STOR %s (for %s)" % (remotepath,file._path) )
        loudeye.storbinary( 'STOR %s' % remotepath,
                            open( file._path, 'rb' ),
                            8192 )

    def frob( self, finalpath, file, city ):
        """
        This should search for and change links to uploads
        to point at loudeye.
        """
	"""
	### this was my old way of doing it, replaced by the froburl trick
        if city == 'nolaimc':
             citydns = 'neworleans'
        elif city == 'prico':
             citydns = 'indymediapr'
	elif city == 'estrecho':
             citydns = 'madiaq'	
        else: citydns = city
	"""

        regex = "http://(www\\.|)%s(\\.org|indymedia\\.org)/uploads/(\d\d\d\d/\d\d/|)%s" % (froburl,escape(file.filename()))
        logger.log( "regex is %s" % regex )
        open( finalpath, 'w' ).write( re.sub( regex, "http://images.indymedia.org/imc/%s/%s" % (city,file.filename()), data ) )


class MP3Publisher( Publisher ):

    def __init__( self ):
        Publisher.__init__(self)

    def upload( self,
    		file,
                city,
                remotepath,
                connection ):
        Publisher.upload( self,file,city,remotepath,connection )

        logger.log( '  detected mp3 upload; making m3u as well.' )
        m3u = StringIO.StringIO( 'http://images.indymedia.org/imc/%s/%s\n' % (city,
                                                                             file.filename()))
        loudeye.storbinary( 'STOR /imc/%s/%s' % (city,file.filename()[:-3]+'m3u'),
                            m3u,
                            8192 )

    def frob( self, finalpath, file, city ):
        regex = "http://(www\\.|)%s\\.(indymedia|ods)\\.(ca|org)/uploads/(\d\d\d\d/\d\d/|)%s" % (city,escape(file.filename()))
        finalname = file.filename()[:-3] + 'm3u'
        logger.log( "  detected mp3 file; auto-generating m3u extension." )
        open( finalpath, 'w' ).write( re.sub( regex, "http://images.indymedia.org/imc/%s/%s" % (city,finalname), data ) )
        


##
## construct publisher objects if EXTPublisher exists,
## it will be used to publish files of extension ``ext''
## (e.g. MP3Publisher)
##

publishers = {}
for ext in MIRROR_EXTENSIONS:
    upext = string.upper( ext )
    pub = None
    try:
        pub = eval( upext + "Publisher()" )
        print "found custom publisher for", upext
    except:
        print "using default publisher for", upext
        pub = Publisher()
    publishers[ ext ] = pub


##
## parse command-line options
##

(options,parameters) = getopt.getopt( sys.argv[1:], [], ["dummy", "force", "tinyfiles",
                                                         "upload-only", "stdout","verbose" ] )

for (option,value) in options:
    if option == '--dummy':
        DUMMY = 1
    elif option == '--force':
        FORCE = 1
    elif option == '--tinyfiles':
        TINYFILES = 1
    elif option == '--upload-only':
        UPLOADONLY = 1
    elif option == '--stdout':
        STDOUT = 1
    elif options == '--verbose':
        VERBOSE = 1


##
## define some helper classes and functions
##

class Logger:
    """
    One instance of this class will exist to log messages
    into the log file.
    """
    
    def __init__( self, name=None ):
        try:
            if name:
                self.file = open( name, 'a' )
            else:
                self.file = sys.stderr
        except IOError:
            print "FATAL: failed to open log file ``" + name + "''"
            sys.exit(-1)

    def log( self, message ):
        tm = time.asctime(time.localtime(time.time()))
        text = '[' + str(tm) + '] ' + string.strip(message)
        self.file.write( text + '\n' )

    def fatal( self, message, level=-2 ):
        self.log( message )
        if level < 0:
            try:
                os.unlink( PID_FILE )
            except:
                logger.log( "WARNING: couldn't delete pid file %s" % PID_FILE )
        self.log( "...exiting" )
        sys.exit(level)


## make a global logger instance
if STDOUT:
    logger = Logger()
else:
    logger = Logger( LOG_FILE )
logger.log( "pytoeye starting..." )

class File:
    """
    A local file instance, which has nice information about files in
    the local upload place.
    """

    def __init__( self, path ):
        if not os.path.isabs(path):
            logger.fatal( "path names must be absolute (%s)." % path )
        if not os.path.isfile(path):
            logger.fatal( "%s is not a file." % path )

        self._path = path
        self._modified = os.path.getmtime( path )
        self._size = os.path.getsize( path )
        self._extension = string.lower(string.split(self.filename(),'.')[-1])

        logger.log( "  %s is %d bytes modified last at %s." % (self._path,
                                                               self._size,
                                                               time.asctime(time.localtime(self._modified))) )

    def filename( self ):
        """return the filename bit"""
        return os.path.split(self._path)[-1]

    def remotename( self ):
        """return the name for the remote system"""
        if RENAME_MAP.has_key( self._extension ):
            (directory,file) = os.path.split(self._path)
            newfile = string.split(file,'.')[0]
            newfile = newfile + '.' + RENAME_MAP[ self._extension ]
            return newfile
        return self.filename()


def checktype( fname ):
    """
    make sure the filename passed in (which doesn't have a path),
    has an ending listed in MIRROR_EXTENSIONS (case insensitive).
    """
    if string.lower(string.split(fname,'.')[-1]) in MIRROR_EXTENSIONS:
        return 1
    elif mimetypes.guess_type(fname)[0] in MIRROR_TYPES:
        return 1
    return 0


def findfiles( dir, newer=None ):
    """
    Recursively find files starting at dir. if newer isn't
    None, then make sure the file is newer than that time.
    Ignores files with the wrong extension, too.
    """

    if FORCE: newer = None
    
    rtn = []
    for name in os.listdir( dir ):
        file = os.path.join(dir,name)
        if os.path.isdir(file):
	    """
	    we don't actually want to recurse directories (yet),
	    this seems to cause problems
	    """
##	    continue
##   actually, this works now for sf-active >= 0.92 -- mtoups
##   the "newer" argument below is the critical bugfix
            rtn = rtn + findfiles( file , newer)
        elif os.path.isfile(file):
            if newer:
                if os.path.getmtime(file) < newer:
                    if VERBOSE:
		        logger.log( "Ignoring %s because it's too old." % file )
                    continue
            (path,fname) = os.path.split(file)
            if not checktype(file):
                if VERBOSE:
		    logger.log( "Ignoring %s because it's not a desired type." % file )
                continue
            rtn.append( File(file) )
        else:
            logger.log( "Ignoring %s because it's neither a file nor a directory." % file )
    return rtn



##
## start the real work
##

##
## see if anyone else is running
##

if os.path.exists(PID_FILE):
    try:
        pid = string.strip(open(PID_FILE,'r').read())
        sys.stderr.write( "Looks like another pytoeye.py script is running at pid %s. If not, please delete %s and try again.\n" % (pid,PID_FILE) )
        logger.fatal( "pytoeye.py is already running at PID %s; if not, delete %s and try again." % (pid,PID_FILE),
                      level=100 )
    finally:
        sys.exit(-10)

pid = open( PID_FILE, 'w' )
pid.write( str(os.getpid()) )
pid.close()


##
## nope; log what the options were
##

if DUMMY:
    logger.log( "Dummy run specified; nothing will be uploaded." )



##
## outer loop goes over all cities
##

for city in CITYNAMES:

    dbpass = DATABASE_PASSWORD_RE.search( open('/www/sf-active/%s/local/config/sfactive.cfg'%city,'r').read() )
    if dbpass:
        dbpass = dbpass.group(1)
    else:
        dbpass = city + '_db'
    froburl = FROB_URL_RE.search( open('/www/sf-active/%s/local/config/sfactive.cfg'%city,'r').read() )
    if froburl:
        froburl = froburl.group(2)
    else:
        froburl = city

    ## you like need to change this, but probably fine as-is
    UPLOADS = UPLOAD_TEMPLATE % (city,)
    PYTOEYE_STATUS = os.path.join( UPLOADS, 'pytoeye-status' )



##
## start the real work. discover some files to mirror
##


    if os.path.isfile( PYTOEYE_STATUS ):
        allfiles = findfiles( UPLOADS, os.path.getmtime(PYTOEYE_STATUS) )
    else:
        allfiles = findfiles( UPLOADS )
    os.system( 'touch %s' % PYTOEYE_STATUS )


##
## okay, at this point allfiles is a list of File objects
## which are newer than PYTOEYE_STATUS (if it exists) and
## have one of the extensions in MIRROR_EXTENSIONS
##
## now, we'll connect to loudeye...
##


    import ftplib
    loudeye = None
    if not DUMMY:
        loudeye = ftplib.FTP( FTP_URL )
        try:
            password = string.strip(open(FTP_PASSWORD_FILE, 'r').read())
            loudeye.login( FTP_USER, password )
        except IOError:
            logger.fatal( "couldn't read loudeye password file %s." % (FTP_PASSWORD_FILE,))
        except ftplib.error_perm:
            logger.log( 'failed to connect to %s (permission denied).' % FTP_URL )
        except:
            logger.log( 'failed to connect to %s.' % FTP_URL )
        logger.log( 'Connected to %s.' % FTP_URL )

    else:
        logger.log( "Not connecting to %s; --dummy specified." % FTP_URL )


##
## change directories (mostly for safety; absolute paths as
## specified all the time anyway) and get a list of all files
## for this city
##

    remotedir = '/imc/%s/' % city
    try:
        if loudeye: loudeye.cwd( remotedir )
        if loudeye: loudeye.set_pasv( 0 )
        logger.log( 'Changed to %s on %s.' % (remotedir,FTP_URL) )
    except:
        logger.fatal( 'Failed to change to %s on %s.' % (remotedir,FTP_URL) )

    loudeyefiles = []
    try:
        if loudeye:
            loudeyefiles = loudeye.nlst( '/%s' % city )
            loudeyefiles = loudeyefiles + loudeye.nlst( '/imc/%s' % city )
    except:
        pass
    logger.log( 'Found %d files (both streaming and non) for city %s.' % (len(loudeyefiles),
                                                                          city))
##
## remove all the paths from loudeye file list
## (i.e. change /city/foo.rm to just foo.rm, etc.)
##
    loudeyefiles = map(lambda x: os.path.split(x)[-1], loudeyefiles)


##
## go through all the files and see if they should be
## tranferred
##

    uploadedfiles = []
    uploadedstreams = []

    for file in allfiles:

        if file._extension in STREAM_EXTENSIONS:
            remotepath = '/%s/%s' % (city,file.remotename())
            uploadedstreams.append( (file,remotepath) )
        else:
            remotepath = '/imc/%s/%s' % (city,file.remotename())
            uploadedfiles.append( (file,remotepath) )

                                        # if the file exists in its
                                        # mapped-name form on the
                                        # remote system, we skip if
                                        # IFF the sizes match
                                        #
                                        # WARNING: presumes that the remote
                                        # system supports SIZE

        if file.remotename() in loudeyefiles and not DUMMY and not FORCE:
            size = loudeye.size( remotepath )
            if size == file._size:
                logger.log( "Not transfering %s; it already exists on %s (as %s)." % (file._path,
                                                                                      FTP_URL,
                                                                                      remotepath))
                continue

                                        # according to jb@riseup.net,
                                        # some systems use placeholder
                                        # files of <= 100 bytes, so
                                        # mirroring those will cause
                                        # the real files to be
                                        # overwritten on loudeye. this
                                        # prevents that (so don't do this
                                        # step even if --force was specified)
        if file._size <= 100:
            if TINYFILES:
                logger.log( "Transferring %s even though it is <= 100 bytes; --tinyfiles specified." % file.filename() )
            else:
                logger.log( "Not transferring %s; it is <= 100 bytes." % file.filename() )
                continue

                                        # transfer the file and time
                                        # it
        start = time.time()
        try:
            if not DUMMY:
                publishers[ file._extension ].upload( file,
                                                      city,
                                                      remotepath,
                                                      loudeye )


            else:
                logger.log( '  would have written to %s' % remotepath )
        except:

            logger.log( "Failed to transfer %s to %s:%s: %s" % (file._path,
                                                             FTP_URL,
                                                             remotepath, sys.exc_value) )
            sys.stderr.write( "%s: %s.\n" % (file._path, sys.exc_value) )


##	    logger.log( "Failed to transfer %s to %s:%s." % (file._path,
##                                                             FTP_URL,
##                                                             remotepath) )
##            sys.stderr.write( "Failed to transfer %s to %s:%s.\n" % (file._path,
##                                                                   FTP_URL,
##                                                                   remotepath) )
            continue
        end = time.time()
        logger.log( 'Transferred "%s" to "%s" in %d seconds.' % (file._path,
                                                                 remotepath,
                                                                 (end-start)) )

    logger.log( "Connecting to database to update cache files." )

    if UPLOADONLY:
        logger.log( "--upload-only specified; not frobbing .PHP files." )
        continue                        # go to next city

##
## uploading for this city complete; frob the database.
##
    if city == 'print':
        dbuser = 'imc_print'
##    elif city == 'prico':
##        dbuser = 'indymediapr'
    else: dbuser = city

    con = MySQLdb.connect( db=city, user=dbuser, passwd = dbpass, host= DBHOST )
    cur = con.cursor( MySQLdb.cursors.DictCursor )

    logger.log( "There are %d non-streaming files to frob." % len(uploadedfiles) )

    for (file,loudeyepath) in uploadedfiles:
        sqlquery = "SELECT date_entered , id , arttype , parent_id from webcast where linked_file = '%s';" % file._path
        logger.log( "Frobbing %s with '%s'." % (loudeyepath,sqlquery) )
        cur.execute( sqlquery )
        if len(cur.fetchall()):
            if len(cur.fetchall()) > 1:
                logger.log( "WARNING: got more than one result; expect 0 or 1." )
            row = cur.fetchall()[0]
            date = row[ 'date_entered' ]
            filedate = string.replace( string.split(date)[0], "-", "/" )[:-2]
            finalpath = os.path.join( (CACHE_TEMPLATE % city), filedate )
            if row[ 'arttype' ] == 'attachment':
                finalpath = os.path.join( finalpath, '%s_comment.php' % row[ 'parent_id' ] )
            else:
                finalpath = os.path.join( finalpath, '%s_comment.php' % row[ 'id' ] )

            def escape( s ):
                rtn = ''
                for x in s:
                    if x == '.':
                        rtn = rtn + '\\.'
                    else:
                        rtn = rtn + x
                return rtn

            logger.log( "final path is %s" % finalpath )
            try:
                data = open( finalpath, 'r' ).read()
            except:
                logger.log( "Failed to open %s; continuing." % finalpath )
                continue

            if not publishers.has_key(file._extension):
                logger.log( "WARNING: no publisher for extension %s; not doing anything." % file._extension )
                continue

            publishers[ file._extension ].frob( finalpath, file, city )

            # this is for the non-comments file -- mtoups
            finalpath = finalpath[:-12] + '.php'

            try:
                data = open( finalpath, 'r' ).read()
            except:
                logger.log( "Failed to open %s; continuing." % finalpath )
                continue
            publishers[ file._extension ].frob( finalpath, file, city )
            

    logger.log( "There are %d streaming files to frob." % len(uploadedstreams) )

    for (file,loudeyepath) in uploadedstreams:
        sqlquery = "SELECT date_entered , id , arttype , parent_id from webcast where linked_file = '%s';" % file._path
        logger.log( "Frobbing %s with '%s'." % (loudeyepath,sqlquery) )
        cur.execute( sqlquery )
        if len(cur.fetchall()):
            if len(cur.fetchall()) > 1:
                logger.log( "WARNING: got more than one result; expect 0 or 1." )
            row = cur.fetchall()[0]
            date = row[ 'date_entered' ]
            filedate = string.replace( string.split(date)[0], "-", "/" )[:-2]
            finalpath = os.path.join( (CACHE_TEMPLATE % city), filedate )
            if row[ 'arttype' ] == 'attachment':
                finalpath = os.path.join( finalpath, '%s_comment.php' % row[ 'parent_id' ] )
            else:
                finalpath = os.path.join( finalpath, '%s.php' % row[ 'id' ] )

            def escape( s ):
                rtn = ''
                for x in s:
                    if x == '.':
                        rtn = rtn + '\\.'
                    else:
                        rtn = rtn + x
                return rtn

            logger.log( "final path is %s" % finalpath )
            try:
                data = open( finalpath, 'r' ).read()
            except:
                logger.log( "Failed to open %s; continuing." % finalpath )
                continue

            remotefinal = file.remotename()
            if remotefinal[-2:] == 'rm':
                remotefinal = remotefinal[:-2] + 'ram'
                logger.log( "  detected RealMedia file %s." % remotefinal )

	    """
	    if city == 'nolaimc':
	       citydns = 'neworleans'
	    elif city == 'prico':
	       citydns = 'indymediapr'
	    else: citydns = city
	    """

            regex = "http://(www\\.|)%s(\\.org|indymedia\\.org)/uploads/(\d\d\d\d/\d\d/|)%s" % (froburl,escape(remotefinal))
            logger.log( "regex is %s" % regex )

            finalname = file.remotename()
            if file._extension == 'mp3':
                finalname = file.remotename()[:-3] + 'm3u'
                logger.log( "  detected mp3 file; auto-generating m3u extension." )
#            if finalname[-2:] == 'rm':
#                finalname = finalname[:-2] + 'ram'
            logger.log( "  finalname is %s" % finalname )
            open( finalpath, 'w' ).write( re.sub( regex, "http://images.indymedia.org/imc/%s/%s" % (city,finalname), data ) )



###
### delete the pid file, since everything went according to plan
###

try:
    os.unlink( PID_FILE )
except:
    logger.log( "WARNING: couldn't delete pid file %s" % PID_FILE )

##
## done all cities; exit
##
        
logger.log( "...exiting." )    
