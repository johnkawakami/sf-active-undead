#!/usr/bin/local/perl -w

use DBI;
use Time::Local;
use File::Basename;

sub mysql_escape_string($) {
  my $string = shift;
  $string =~ s/\\/\\\\/g;   
  $string =~ s/\n/\\n/g;
  $string =~ s/\r/\\r/g;
  $string =~ s/'/\\'/g; 
  $string =~ tr/\x93\x94/""/;
  $string =~ s/\x92/\\'/g;  
  return $string;
}

($#ARGV == 0) or die "usage: pg.pl cityname";

my $city = $ARGV[0];
# new image directory (on new server if you have one)  
my $uploads_dir_new = "/home/melbimc/www/sf-active/melbimc/webcast/uploads";
# current directory for text uploads, not metafiles directory
my $uploads_dir_current = "/www/uploads/$city";
my $dsn = "dbi:Pg:dbname=active_$city host=inglis port=6543";
# find the username / password in your local/db_setup.php3 file
my $user = 'melbourne';
my $pass = '';


my $months = {
'Jan' => '01',
'Feb' => '02',
'Mar' => '03',
'Apr' => '04',
'May' => '05',
'Jun' => '06',
'Jul' => '07',
'Aug' => '08',
'Sep' => '09',
'Oct' => '10',
'Nov' => '11',
'Dec' => '12'
};


my $dbh = DBI->connect($dsn, $user, $pass) or die "Cant connect to the
DB: $DBI::errstr\n";

my $sth = $dbh->prepare( "SELECT * FROM webcast WHERE ORDER BY id DESC" ) or die "Can't prepare SQL statement: ", $dbh->errstr(), "\n"; 

$sth->execute or die "Can't execute SQL statement: ", $sth->errstr(), "\n";

my $max_id=0;

print "ALTER TABLE webcast MODIFY id int(11) NOT NULL;\n";

while ( my @fields = $sth->fetchrow_array() ) {

  $max_id = $fields[0] if $fields[0] > $max_id;

  #Grab "html_file" and put it in the database
  if ($fields[14] =~ /^\S/) {
    fileparse_set_fstype('VMS');
    $basename = basename($fields[14]);
    if (! open INFILE, "$uploads_dir_current/$basename") {
      print STDERR "$uploads_dir_current/$basename: $!\n";
      $fields[4] = 'No article available';
    }
    else {
      $fields[4] = join '', <INFILE>; 
    }
    close INFILE;  
  }

  #Convert postgres timestamps to mysql timestamps, completely ignoring time-zones
  $fields[15] =~ /(\d\d\d\d)-(\d\d)-(\d\d) (\d\d):(\d\d):(\d\d)/ or die "Malformed date string at id=".$fields[0]." ".$fields[15];
  $fields[15] = $1.$2.$3.$4.$5.$6;
  $fields[3] = "$1-$2-$3 $4:$5:$6";
  $fields[16] =~ /(\d\d\d\d)-(\d\d)-(\d\d) (\d\d):(\d\d):(\d\d)/ or die "Malformed date string at id=".$fields[0];
  $fields[16] = $1.$2.$3.$4.$5.$6;

  #Fix funky characters
  for(my $i=0;$i<=$#fields;$i++) { 
   if ($fields[$i]) {
      $fields[$i] = mysql_escape_string($fields[$i]);
    } 
  }

  #Set "group_status" to empty string - in SF it becomes "rating"
  $fields[20] = '';

  if ($fields[21]) {
  #Convert "is_html" to "artmime"
    if ($fields[21]==1) {
      $fields[21] = 'h';
      $fields[14] = 'text/html';
    }
    elsif ($fields[21]==0) {
      $fields[21] = 't';
      $fields[14] = 'text/plain';
    }
    #If, for some reason, this field has a weird value
    else {
      if ($fields[4] =~ /<br>/i || ($fields[4] =~ /<p>/i )) {
        $fields[21] = 'h';
        $fields[14] = 'text/html';
      }
      else {
        $fields[21] = 't';
        $fields[14] = 'text/plain';
      }
    }
  }
  #If, for some reason, this field has no value
  else {
    if ($fields[4] =~ /<br>/i || ($fields[4] =~ /<p>/i )) {
      $fields[21] = 'h';
      $fields[14] = 'text/html';
    }
    else {
      $fields[21] = 't';
      $fields[14] = 'text/plain';
    }
  }

  #Change "linked_file" to reflect the SF structure

  $fields[17] =~ s/^\s*(.*?)\s*$/$1/;
  if ($fields[17] ne '') {
    fileparse_set_fstype('VMS');
    my $basename = basename($fields[17]);
    $fields[17] = "$uploads_dir_new/$basename";
  }
  if ($fields[19] eq '1') {
     $fields[19] = 't';
  } else {
     $fields[19] = 'f';
  } 
   

  #Put the fields back together
  my $line = "INSERT INTO webcast VALUES ($fields[0],'$fields[1]','$fields[2]','$fields[3]','$fields[4]','$fields[5]','$fields[6]','$fields[7]','$fields[8]',$fields[9],'$fields[10]','$fields[11]',$fields[12],'$fields[13]','$fields[14]',$fields[15],$fields[16],'$fields[17]','$fields[18]','$fields[19]','$fields[20]','$fields[21]');";

  #Print the line
  print "$line\n";
}
warn "Problem in fetchrow_array(): ", $sth->errstr(), "\n" if $sth->err();

print "ALTER TABLE webcast MODIFY id int(11) NOT NULL auto_increment;\nSET INSERT_ID = $max_id;\n";

$dbh->disconnect; 


